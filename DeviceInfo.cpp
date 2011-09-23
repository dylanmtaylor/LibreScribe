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

#include "wx_pch.h"
#include "DeviceInfo.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(DeviceInfo)
	#include <wx/string.h>
	#include <wx/intl.h>
	//*)
#endif
//(*InternalHeaders(DeviceInfo)
#include <wx/font.h>
#include <wx/bitmap.h>
#include <wx/image.h>
//*)

//(*IdInit(DeviceInfo)
const long DeviceInfo::idInformationIcon = wxNewId();
const long DeviceInfo::idDeviceTypeText = wxNewId();
const long DeviceInfo::idDeviceNameLabel = wxNewId();
const long DeviceInfo::idDeviceName = wxNewId();
const long DeviceInfo::idBatteryLabel = wxNewId();
const long DeviceInfo::idBatteryRemainingGauge = wxNewId();
const long DeviceInfo::idStorageLabel = wxNewId();
const long DeviceInfo::idStorageRemaining = wxNewId();
//*)

BEGIN_EVENT_TABLE(DeviceInfo,wxDialog)
	//(*EventTable(DeviceInfo)
	//*)
END_EVENT_TABLE()

DeviceInfo::DeviceInfo(wxWindow* parent, uint16_t productID, Smartpen* smartpen, wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(DeviceInfo)
	wxFlexGridSizer* mainSizer;
	wxFlexGridSizer* informationSizer;
	wxFlexGridSizer* dialogSizer;

	Create(parent, wxID_ANY, _("Smartpen Device Information"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	SetClientSize(wxSize(600,165));
	SetMinSize(wxSize(585,165));
	SetMaxSize(wxSize(585,165));
	mainSizer = new wxFlexGridSizer(0, 2, 0, 0);
	infoIcon = new wxStaticBitmap(this, idInformationIcon, wxBitmap(wxImage(_T("res/dialog-information-128.png")).Rescale(wxSize(128,128).GetWidth(),wxSize(128,128).GetHeight())), wxDefaultPosition, wxSize(128,128), 0, _T("idInformationIcon"));
	mainSizer->Add(infoIcon, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	dialogSizer = new wxFlexGridSizer(0, 1, 0, 0);
	deviceType = new wxStaticText(this, idDeviceTypeText, _("LightScribe Pulse(TM) Smartpen"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT, _T("idDeviceTypeText"));
	wxFont deviceTypeFont(13,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Sans"),wxFONTENCODING_DEFAULT);
	deviceType->SetFont(deviceTypeFont);
	dialogSizer->Add(deviceType, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	informationSizer = new wxFlexGridSizer(2, 2, 0, 0);
	nameLabel = new wxStaticText(this, idDeviceNameLabel, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("idDeviceNameLabel"));
	informationSizer->Add(nameLabel, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	deviceName = new wxStaticText(this, idDeviceName, _("Dylan Taylor\'s Smartpen"), wxDefaultPosition, wxDefaultSize, 0, _T("idDeviceName"));
	informationSizer->Add(deviceName, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	batteryLabel = new wxStaticText(this, idBatteryLabel, _("Battery:"), wxDefaultPosition, wxDefaultSize, 0, _T("idBatteryLabel"));
	informationSizer->Add(batteryLabel, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	batteryGauge = new wxGauge(this, idBatteryRemainingGauge, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("idBatteryRemainingGauge"));
	batteryGauge->SetValue(50);
	batteryGauge->SetMinSize(wxSize(250,25));
	batteryGauge->SetMaxSize(wxSize(250,25));
	informationSizer->Add(batteryGauge, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	storageLabel = new wxStaticText(this, idStorageLabel, _("Storage:"), wxDefaultPosition, wxDefaultSize, 0, _T("idStorageLabel"));
	informationSizer->Add(storageLabel, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	storageRemaining = new wxStaticText(this, idStorageRemaining, _("1669.0 of 1925.0 MiB (86.70%) remaining."), wxDefaultPosition, wxDefaultSize, 0, _T("idStorageRemaining"));
	informationSizer->Add(storageRemaining, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	dialogSizer->Add(informationSizer, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	mainSizer->Add(dialogSizer, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 10);
	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);
	//*)

    this->smartpen = smartpen;
    const char* s = smartpen->getInfo();
    printf("%s\n",s);
    xmlDocPtr doc = xmlParseMemory(s, strlen(s));
    xmlNode *cur = xmlDocGetRootElement(doc);
    int batteryLevel = getBatteryRemaining(cur);
    printf("batteryLevel: %d\n", batteryLevel);
    batteryGauge->SetValue(batteryLevel);
    unsigned long long int freeBytes = getFreeBytes(cur);
    unsigned long long int totalBytes = getTotalBytes(cur);
    char fs[256];
    float freeMiB = convertBytesToMiB(freeBytes);
    float totalMiB = convertBytesToMiB(totalBytes);
    float pctRemaining = (freeMiB/totalMiB) * 100;
    sprintf(fs, "%.02f of %.02f MiB (%.02f%%) remaining.\n",freeMiB,totalMiB,pctRemaining); //stores formatted string in fs
    printf("%s",fs); //displays fs on stdout
    printf("Setting device name label\n");
    const char* n = smartpen->getName();
    printf("%s\n",n);
    wxString devName(n, wxConvUTF8);
    deviceName->SetLabel(devName);
    if (is_ls_pulse(productID)) {
        deviceType->SetLabel(_("LiveScribe Pulse(TM) Smartpen"));
    } else if (is_ls_echo(productID)) {
        deviceType->SetLabel(_("LiveScribe Echo(TM) Smartpen"));
    } else {
        deviceType->SetLabel(_("Unknown LiveScribe Device"));
        printf("Unable to determine device type\n");
    }
    printf("device name label set, setting battery gauge value\n");
    printf("battery gauge value set, setting storage remaining label\n");
    wxString freeSpace(fs, wxConvUTF8);
    storageRemaining->SetLabel(freeSpace);
    printf("success! we made it through the device information constructor unscathed!\n");
}

DeviceInfo::~DeviceInfo()
{
	//(*Destroy(DeviceInfo)
	//*)
}

//This function will strip out all non-numeric characters from a char*
unsigned long long int stripNonNumericChars(const char* s) {
    unsigned long long int res;
    bool success = sscanf(s, "%llu%%", &res) == 1;
    if (!success) {
        printf("Error matching '%s'", s);
    }
//    printf("after stripping non-numeric characters: %llu\n", res);
    return res;
}

static void showBatteryStatistics(xmlNode * a_node) {
    xmlNode *cur_node = NULL;
    //scan for battery element
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (strcmp((const char*)cur_node->name,"battery") == 0) {
                char* voltage = (char*)xmlGetProp(cur_node, (const xmlChar*)"voltage");
                char* level = (char*)xmlGetProp(cur_node, (const xmlChar*)"level");
                printf("Battery Level: %s, Voltage: %s\n", level, voltage);
                return;
            }
        }
        showBatteryStatistics(cur_node->children);
    }
}

//searches through a node until an element with the specified node is detected.
xmlNode * getSubNode(xmlNode *root, const xmlChar *node) {
    for(xmlNode *cur_node = root->children; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(cur_node->name, node)) {
            return cur_node;
        }
    }
    assert(false);
    return 0;
}

float DeviceInfo::convertBytesToMiB(unsigned long long int bytes) {
    float mib = (bytes / 1048576);  //there are 1,048,576 bytes in one megabyte.
    printf("converting %llu bytes to MiB: %.02f MiB\n",bytes,mib);
    return mib;
}

float DeviceInfo::getBatteryVoltage(xmlNode *root) {
    //first we need to locate the "peninfo" node which contains the "battery" node
    xmlNode *cur_node = getSubNode(root, (const xmlChar *)"peninfo");
    //once we find our "peninfo" node, search through the nodes, looking for the "battery" node
    cur_node = getSubNode(cur_node, (const xmlChar *)"battery");
    char* voltage = (char*)xmlGetProp(cur_node, (const xmlChar*)"voltage");
    return stripNonNumericChars(voltage);
}

int DeviceInfo::getBatteryRemaining(xmlNode *root) {
    //first we need to locate the "peninfo" node which contains the "battery" node
    xmlNode *cur_node = getSubNode(root, (const xmlChar *)"peninfo");
    //once we find our "peninfo" node, search through the nodes, looking for the "battery" node
    cur_node = getSubNode(cur_node, (const xmlChar *)"battery");
    char* level = (char*)xmlGetProp(cur_node, (const xmlChar*)"level");
    return stripNonNumericChars(level);
}

unsigned long long int DeviceInfo::getFreeBytes(xmlNode *root) {
    //first we need to locate the "peninfo" node which contains the "memory" node
    xmlNode *cur_node = getSubNode(root, (const xmlChar *)"peninfo");
    //once we find our "peninfo" node, search through the nodes, looking for the "memory" node
    cur_node = getSubNode(cur_node, (const xmlChar *)"memory");
    char* bytes = (char*)xmlGetProp(cur_node, (const xmlChar*)"freebytes");
    printf("free bytes: %s\n", bytes);
    return stripNonNumericChars(bytes);
}

unsigned long long int DeviceInfo::getTotalBytes(xmlNode *root) {
    //first we need to locate the "peninfo" node which contains the "memory" node
    xmlNode *cur_node = getSubNode(root, (const xmlChar *)"peninfo");
    //once we find our "peninfo" node, search through the nodes, looking for the "memory" node
    cur_node = getSubNode(cur_node, (const xmlChar *)"memory");
    char* bytes = (char*)xmlGetProp(cur_node, (const xmlChar*)"totalbytes");
    printf("total bytes: %s\n", bytes);
    return stripNonNumericChars(bytes);
}

