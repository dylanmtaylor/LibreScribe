#ifndef DEVICEINFO_H
#define DEVICEINFO_H

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

		DeviceInfo(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~DeviceInfo();

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

		//(*Identifiers(DeviceInfo)
		static const long ID_STATICBITMAP1;
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_GAUGE1;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT2;
		//*)

	private:

		//(*Handlers(DeviceInfo)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
