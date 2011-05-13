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
const long DeviceInfo::ID_STATICBITMAP1 = wxNewId();
const long DeviceInfo::ID_STATICTEXT1 = wxNewId();
const long DeviceInfo::ID_STATICTEXT3 = wxNewId();
const long DeviceInfo::ID_STATICTEXT4 = wxNewId();
const long DeviceInfo::ID_STATICTEXT5 = wxNewId();
const long DeviceInfo::ID_GAUGE1 = wxNewId();
const long DeviceInfo::ID_STATICTEXT6 = wxNewId();
const long DeviceInfo::ID_STATICTEXT2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(DeviceInfo,wxDialog)
	//(*EventTable(DeviceInfo)
	//*)
END_EVENT_TABLE()

DeviceInfo::DeviceInfo(wxWindow* parent, wxString devName, uint16_t productID, obex_t *handle, wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(DeviceInfo)
	wxFlexGridSizer* mainSizer;
	wxFlexGridSizer* informationSizer;
	wxFlexGridSizer* dialogSizer;

	Create(parent, wxID_ANY, _("Smartpen Device Information"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	SetClientSize(wxSize(585,165));
	SetMinSize(wxSize(585,165));
	SetMaxSize(wxSize(585,165));
	mainSizer = new wxFlexGridSizer(0, 3, 0, 0);
	infoIcon = new wxStaticBitmap(this, ID_STATICBITMAP1, wxBitmap(wxImage(_T("/home/dylan/cpp/LibreScribe/res/dialog-information-128.png")).Rescale(wxSize(128,128).GetWidth(),wxSize(128,128).GetHeight())), wxDefaultPosition, wxSize(128,128), 0, _T("ID_STATICBITMAP1"));
	mainSizer->Add(infoIcon, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	dialogSizer = new wxFlexGridSizer(0, 1, 0, 0);
	deviceType = new wxStaticText(this, ID_STATICTEXT1, _("LightScribe Pulse(TM) Smartpen"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT, _T("ID_STATICTEXT1"));
	wxFont deviceTypeFont(13,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Sans"),wxFONTENCODING_DEFAULT);
	deviceType->SetFont(deviceTypeFont);
	dialogSizer->Add(deviceType, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	informationSizer = new wxFlexGridSizer(2, 2, 0, 0);
	nameLabel = new wxStaticText(this, ID_STATICTEXT3, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	informationSizer->Add(nameLabel, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	deviceName = new wxStaticText(this, ID_STATICTEXT4, _("Dylan Taylor\'s Smartpen"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	informationSizer->Add(deviceName, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	batteryLabel = new wxStaticText(this, ID_STATICTEXT5, _("Battery:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	informationSizer->Add(batteryLabel, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	batteryGauge = new wxGauge(this, ID_GAUGE1, 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_GAUGE1"));
	batteryGauge->SetValue(50);
	batteryGauge->SetMinSize(wxSize(250,25));
	batteryGauge->SetMaxSize(wxSize(250,25));
	informationSizer->Add(batteryGauge, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	storageLabel = new wxStaticText(this, ID_STATICTEXT6, _("Storage:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	informationSizer->Add(storageLabel, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	storageRemaining = new wxStaticText(this, ID_STATICTEXT2, _("1.55GB of 2.13GB remaining"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	informationSizer->Add(storageRemaining, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	dialogSizer->Add(informationSizer, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	mainSizer->Add(dialogSizer, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 10);
	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);
	//*)
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
    sprintf(fs, "%d of %d bytes remaining.\n",freeBytes,totalBytes); //stores formatted string in fs
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
}

DeviceInfo::~DeviceInfo()
{
    smartpen_disconnect(device_handle);
	//(*Destroy(DeviceInfo)
	//*)
}

//This function will strip out all non-numeric characters from a char*
int stripNonNumericChars(char* s) {
    int res;
    bool success = sscanf(s, "%d%%", &res) == 1;
    return res;
}

static void showStorageInformation(xmlNode * a_node) {
    xmlNode *cur_node = NULL;
    //scan for battery element
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (strcmp((const char*)cur_node->name,"memory") == 0) {
                //printf("node type: Element, name: %s\n", cur_node->name);
                char* free = (char*)xmlGetProp(cur_node, (const xmlChar*)"freebytes");
                char* total = (char*)xmlGetProp(cur_node, (const xmlChar*)"totalbytes");
                printf("Storage: %.02f of %.02f MB remaining\n", ((float)atoi(free)/(float)1048576), ((float)atoi(total)/(float)1048576));
                return;
            }
        }
        showStorageInformation(cur_node->children);
    }
}

static void showBatteryStatistics(xmlNode * a_node) {
    xmlNode *cur_node = NULL;
    //scan for battery element
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (strcmp((const char*)cur_node->name,"battery") == 0) {
                //printf("node type: Element, name: %s\n", cur_node->name);
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

long long int DeviceInfo::getFreeBytes(xmlNode *root) {
    //first we need to locate the "peninfo" node which contains the "memory" node
    xmlNode *cur_node = getSubNode(root, (const xmlChar *)"peninfo");
    //once we find our "peninfo" node, search through the nodes, looking for the "memory" node
    cur_node = getSubNode(cur_node, (const xmlChar *)"memory");
    char* bytes = (char*)xmlGetProp(cur_node, (const xmlChar*)"freebytes");
    return stripNonNumericChars(bytes);
}

long long int DeviceInfo::getTotalBytes(xmlNode *root) {
    //first we need to locate the "peninfo" node which contains the "memory" node
    xmlNode *cur_node = getSubNode(root, (const xmlChar *)"peninfo");
    //once we find our "peninfo" node, search through the nodes, looking for the "memory" node
    cur_node = getSubNode(cur_node, (const xmlChar *)"memory");
    char* bytes = (char*)xmlGetProp(cur_node, (const xmlChar*)"totalbytes");
    return stripNonNumericChars(bytes);
}

