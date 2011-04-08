#ifndef DEVICEINFORMATION_H
#define DEVICEINFORMATION_H
#include "../GUIFrame.h"
#include "usb_device.h"
#include "Smartpen.h"
#include <libxml/tree.h>
#include <libxml/parser.h>
class DeviceInformation : public DeviceInfo
{
    public:
        DeviceInformation(wxWindow* parent) : DeviceInfo(parent) { //TODO: get actual device status instead of faking it with hard-coded values
//            obex_t *handle = smartpen_connect(dev->descriptor.idVendor, dev->descriptor.idProduct);
//            char* s = smartpen_get_peninfo(handle);
//            xmlDocPtr doc = xmlParseMemory(s, strlen(s));
//            xmlNodePtr cur = xmlDocGetRootElement(doc);
            int battery = 24; //getBatteryRemaining(cur);
            deviceName->SetLabel(_("Dylan Taylor's Smartpen"));
//            switch (dev->descriptor.idProduct) {
//                case LS_PULSE:
                    deviceType->SetLabel(_("LightScribe Pulse(TM) Smartpen"));
//                case LS_ECHO:
//                    deviceType->SetLabel(_("LightScribe Echo(TM) Smartpen"));
//                default:
//                    deviceType->SetLabel(_("Unknown LightScribe Device"));
//            }
            batteryGauge->SetValue(battery);
            storageRemaining->SetLabel(_("1.65GB of 2.13GB remaining"));
        };
        virtual ~DeviceInformation();
    protected:
    private:
        int getBatteryRemaining(xmlNode *a_node);
};

#endif // DEVICEINFORMATION_H
