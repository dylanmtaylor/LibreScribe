#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(DeviceInfo)
	#include <wx/dialog.h>
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
		//*)

	protected:

		//(*Identifiers(DeviceInfo)
		//*)

	private:

		//(*Handlers(DeviceInfo)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
