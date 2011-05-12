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

#ifndef DEVICEINFORMATION_H
#define DEVICEINFORMATION_H
#include "../GUIFrame.h"
#include "Smartpen.h"
#include <libudev.h>
#include <libxml/tree.h>
#include <libxml/parser.h>

class DeviceInformation : public DeviceInfo {
public:
    DeviceInformation(wxWindow* parent, wxString devName, uint16_t productID, obex_t *handle) : DeviceInfo(parent) { //TODO: get actual device status instead of faking it with hard-coded values
        device_handle = handle;
        char* s = smartpen_get_peninfo(device_handle);
        printf("%s\n",s);
        xmlDocPtr doc = xmlParseMemory(s, strlen(s));
        xmlNode *cur = xmlDocGetRootElement(doc);
        int batteryLevel = getBatteryRemaining(cur);
        printf("batteryLevel: %d\n", batteryLevel);
        batteryGauge->SetValue(batteryLevel);
        int freeBytes = getFreeBytes(cur);
        int totalBytes = getTotalBytes(cur);
        char fs[256];
        sprintf(fs, "%d of %d bytes remaining.\n\0",freeBytes,totalBytes); //stores formatted string in fs
        printf("%s",fs); //displays fs on stdout
        printf("Setting device name label\n");
        deviceName->SetLabel(devName);
        if (productID == LS_PULSE) {
            deviceType->SetLabel(_("LiveScribe Pulse(TM) Smartpen"));
        } else if (productID == LS_ECHO) {
            deviceType->SetLabel(_("LiveScribe Echo(TM) Smartpen"));
        } else {
            deviceType->SetLabel(_("Unknown LiveScribe Device"));
            printf("Unable to determine device type\n");
        }
        printf("device name label set, setting battery gauge value\n");
        printf("battery gauge value set, setting storage remaining label\n");
        wxString freeSpace(fs, wxConvUTF8);
        storageRemaining->SetLabel(freeSpace);
        printf("storage remaining label set, disconnecting device\n");
        smartpen_disconnect(device_handle);
        printf("success! we made it through the device information constructor unscathed!\n");
    };
    virtual ~DeviceInformation();
    int getBatteryRemaining(xmlNode *a_node);
    float getBatteryVoltage(xmlNode *a_node);
    long long int getFreeBytes(xmlNode *a_node);
    long long int getTotalBytes(xmlNode *a_node);
protected:
private:
    obex_t *device_handle;
};

#endif // DEVICEINFORMATION_H
