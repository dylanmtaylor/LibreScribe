/* This file is part of LibreScribe.

LibreScribe is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

LibreScribe is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LibreScribe.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SMARTPEN_H_
#define _SMARTPEN_H_

#include <openobex/obex.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <libusb.h>
#include <assert.h>
#include <arpa/inet.h>
#include <usb.h>

#define LS_VENDOR_ID 0x1cfb //LiveScribe Vendor ID
#define LS_PULSE 0x1020 //LiveScribe Pulse(TM) Smartpen
#define LS_ECHO 0x1030 //LiveScribe Echo(TM) Smartpen

static struct usb_device *findSmartpen() {
    struct usb_bus *bus;
    struct usb_device *dev;
    struct usb_bus *busses;

    usb_init();
    usb_find_busses();
    usb_find_devices();
    busses = usb_get_busses();
    for (bus = busses; bus; bus = bus->next) {
        for (dev = bus->devices; dev; dev = dev->next) {
            if ((dev->descriptor.idVendor == LS_VENDOR_ID)) {
                return dev;
            }
        }
    }
    return NULL;
}

struct obex_state {
    obex_t *handle;
    int req_done;
    char *body;
    int body_len;
    int got_connid;
    int connid;
};

obex_t *smartpen_connect(short vendor, short product);
char *smartpen_get_changelist(obex_t *handle, int starttime);
void smartpen_disconnect (obex_t *handle);
int smartpen_get_guid (obex_t *handle, FILE *out, char *guid, long long int start_time);
int smartpen_get_paperreplay (obex_t *handle, FILE *out, long long int start_time);
int smartpen_get_penletlist (obex_t *handle, FILE *out);
char* smartpen_get_peninfo (obex_t *handle);

#endif
