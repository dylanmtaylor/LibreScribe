#include <openobex/obex.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <glib.h>

static int req_done=0;

volatile int loop = 0;

typedef struct {
    obex_t *handle;
    int connid;
    int err;
    char *body;
    int body_len;

} obex_state;

static void obex_requestdone (obex_state *state, obex_t *hdl,
                              obex_object_t *obj, int cmd, int resp)
{
    uint8_t header_id;
    obex_headerdata_t hdata;
    uint32_t hlen;

    printf("Request done\n");

    switch (cmd & ~OBEX_FINAL) {
        case OBEX_CMD_CONNECT:
            printf("cmd connect\n");
            while (OBEX_ObjectGetNextHeader(hdl, obj, &header_id,
                                            &hdata, &hlen)) {
                /* Get the connection identifier */
                printf("header %x\n", header_id);
                if (header_id == OBEX_HDR_WHO) {
                    printf("who: %s\n", hdata.bs);
                }
            }
            break;

        case OBEX_CMD_DISCONNECT:
            printf("cmd disconnect\n");
            break;

        case OBEX_CMD_PUT:
            printf("cmd put\n");
            while (OBEX_ObjectGetNextHeader(hdl, obj, &header_id,
                                            &hdata, &hlen))
                printf("header %d\n", header_id);
            break;

        case OBEX_CMD_GET:
            printf("cmd get\n");
            while (OBEX_ObjectGetNextHeader(hdl, obj, &header_id,
                                            &hdata, &hlen)) {
                printf("header %x\n", header_id);
                if (header_id == OBEX_HDR_BODY || header_id == OBEX_HDR_BODY_END) {
                    printf("Body!\n\n%s\n\n", hdata.bs);
                    if (state->body)
                        free(state->body);
                    state->body = (char*)malloc(hlen);
                    state->body_len = hlen;
                    memcpy(state->body, hdata.bs, hlen);
                    break;
                }
            }
            break;

        default:
            printf("Funny command\n");
    }

    req_done++;
}

static void obex_event (obex_t *hdl, obex_object_t *obj, int mode,
        int event, int obex_cmd, int obex_rsp) {
    obex_state *state;
    obex_headerdata_t hd;
    int size;
    int rc;

    state = (obex_state*)OBEX_GetUserData(hdl);

    if (event == OBEX_EV_PROGRESS) {
        hd.bq4 = 0;
        size = 4;
        rc = OBEX_ObjectAddHeader(hdl, obj, OBEX_HDR_CONNECTION, hd, size, OBEX_FL_FIT_ONE_PACKET);
        if (rc < 0) {
            printf("oah fail %d\n", rc);
        }
        return;
    }

    printf("event!\n");
    if (obex_rsp != OBEX_RSP_SUCCESS && obex_rsp != OBEX_RSP_CONTINUE) {
        printf("FAIL %x %x\n", obex_rsp, event);
        req_done++;
        return;
    }

    switch (event) {
        case OBEX_EV_REQDONE:
            obex_requestdone(state, hdl, obj, obex_cmd, obex_rsp);
            break;

        default:
            printf("Funny event\n");
    }
}

int syncml_obex_send_req () {
    obex_t *handle = NULL;
    obex_object_t *obj;
    obex_state state;
    obex_headerdata_t hd;
    obex_interface_t *obex_intf;
    int size;
    int i;
    glong num;
    char *name;
    int fd;

    if (!handle) {
        handle = OBEX_Init(OBEX_TRANS_USB, obex_event, 0);
        state.handle = handle;
        state.body = NULL;
        if (!handle)
            perror("handle");
        OBEX_SetUserData(handle, &state);

//        num = OBEX_EnumerateInterfaces(handle);
////        for (i=0; i<num; i++) {
////            obex_intf = OBEX_GetInterfaceByIndex(handle, i);
////            if (obex_intf->usb.manufacturer == 0x1cfb &&
////                obex_intf->usb.product == 0x1020)
////            {
////                printf("Found Livescribe Pulse pen\n");
////                break;
////            }
////        }
//        if (i == num) {
//            return 0;
//        }

        i = OBEX_InterfaceConnect(handle, obex_intf);
        printf("connect = %d\n", i);

        OBEX_SetTransportMTU(handle, 0x400, 0x400);

        obj = OBEX_ObjectNew(handle, OBEX_CMD_CONNECT);
        hd.bs = (const uint8_t*)"LivescribeService";
        size = (int)strlen((const char*)hd.bs)+1;
        OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_TARGET, hd, size, 0);

        if (OBEX_Request(handle, obj) < 0)
            printf("request");

        while (req_done < 1) {
            OBEX_HandleInput(handle, 1);
        }
    }

    printf("CONNECTED\n");

    obj = OBEX_ObjectNew(handle, OBEX_CMD_GET);
    hd.bq4 = 0;
    size = 4;
    OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_CONNECTION, hd, size, OBEX_FL_FIT_ONE_PACKET);

    name=(char*)"changelist?start_time=0";

    hd.bs = g_utf8_to_utf16(name,
                            strlen(name),
                             NULL,
                              &num,
                               NULL);

    for (i=0; i<num; i++) {
        uint16_t *wchar = &hd.bs[i*2];
        *wchar = ntohs(*wchar);
    }
    size = (num+1) * sizeof(uint16_t);
    OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_NAME, hd, size, OBEX_FL_FIT_ONE_PACKET);

    if (OBEX_Request(handle, obj) < 0)
        printf("foo2\n");
    while (req_done < 2) {
        OBEX_HandleInput(handle, 1);
    }

    printf("LISTING DONE\n");

    obj = OBEX_ObjectNew(handle, OBEX_CMD_GET);
    hd.bq4 = 0;
    size = 4;
    OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_CONNECTION, hd, size, OBEX_FL_FIT_ONE_PACKET);

    name=(char*)"lspdata?name=0xbc61f02c87e32f9b&start_time=8782700699";
    hd.bs = g_utf8_to_utf16(name, strlen(name), NULL, &num, NULL);

    for (i=0; i<num; i++) {
        uint16_t *wchar = &hd.bs[i*2];
        *wchar = ntohs(*wchar);
    }
    size = (num+1) * sizeof(uint16_t);
    OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_NAME, hd, size, OBEX_FL_FIT_ONE_PACKET);

    if (OBEX_Request(handle, obj) < 0)
        printf("foo2\n");
    while (req_done < 3) {
        OBEX_HandleInput(handle, 100);
    }

    printf("DATA DONE\n");

    fd = open("data", O_WRONLY|O_CREAT);
    if (fd < 0) {
        perror("open");
    }
    write(fd, state.body, state.body_len);
    close(fd);

    obj = OBEX_ObjectNew(handle, OBEX_CMD_DISCONNECT);
    hd.bq4 = 0;
    size = 4;
    OBEX_ObjectAddHeader(handle, obj, OBEX_HDR_CONNECTION, hd, size, OBEX_FL_FIT_ONE_PACKET);

    if (OBEX_Request(handle, obj) < 0)
        printf("foo2\n");
    while (OBEX_HandleInput(handle, 100) == 0);

    return 1;
}
//
//int main (int argc, char **argv) {
//    char *data = "This is some exciting data!";
//    int ret;
//
//    ret = syncml_obex_send_req(data, strlen(data), NULL, "x-obex/folder-listing");
//
//    if (!ret)
//        printf("ugh\n");
//
//    /*while (!loop) {
//        loop = 0;
//    }*/
//    return 0;
//}
