#ifndef _SMARTPEN_H_
#define _SMARTPEN_H_

#include <openobex/obex.h>

class smartpen
{
    public:
        smartpen();
        virtual ~smartpen();
        obex_t *smartpen_connect(short vendor, short product);
        char *smartpen_get_changelist(obex_t *handle, int starttime);
        void smartpen_disconnect (obex_t *handle);
        int smartpen_get_guid (obex_t *handle, FILE *out, char *guid, long long int start_time);
        int smartpen_get_paperreplay (obex_t *handle, FILE *out, long long int start_time);
        int smartpen_get_penletlist (obex_t *handle, FILE *out);
        char *smartpen_get_peninfo (obex_t *handle);
    protected:
    private:
};

#endif
