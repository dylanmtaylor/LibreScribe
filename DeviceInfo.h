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

#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <libudev.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>
#include "Smartpen.h"
#ifndef WX_PRECOMP
	//(*HeadersPCH(DeviceInfo)
	#include <wx/gauge.h>
	#include <wx/dialog.h>
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/statbmp.h>
	//*)
#endif
//(*Headers(DeviceInfo)
//*)

class DeviceInfo: public wxDialog
{
	public:

        DeviceInfo(wxWindow* parent, uint16_t productID, obex_t *handle, wxWindowID id=wxID_ANY, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize);
		virtual ~DeviceInfo();
        float convertBytesToMiB(long long int bytes);
		//(*Declarations(DeviceInfo)
		wxGauge* batteryGauge;
		wxStaticText* storageRemaining;
		wxStaticText* deviceType;
		wxStaticText* deviceName;
		wxStaticBitmap* infoIcon;
		wxStaticText* storageLabel;
		wxStaticText* nameLabel;
		wxStaticText* batteryLabel;
		//*)

	protected:

        obex_t *device_handle;

		//(*Identifiers(DeviceInfo)
		static const long idInformationIcon;
		static const long idDeviceTypeText;
		static const long idDeviceNameLabel;
		static const long idDeviceName;
		static const long idBatteryLabel;
		static const long idBatteryRemainingGauge;
		static const long idStorageLabel;
		static const long idStorageRemaining;
		//*)

	private:

        int getBatteryRemaining(xmlNode *a_node);
        float getBatteryVoltage(xmlNode *a_node);
        long long int getFreeBytes(xmlNode *a_node);
        long long int getTotalBytes(xmlNode *a_node);

		//(*Handlers(DeviceInfo)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
