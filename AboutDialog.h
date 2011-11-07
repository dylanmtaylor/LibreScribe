#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(AboutDialog)
	#include <wx/dialog.h>
	#include <wx/sizer.h>
	#include <wx/stattext.h>
	#include <wx/statbmp.h>
	//*)
#endif
//(*Headers(AboutDialog)
#include <wx/statline.h>
//*)

class AboutDialog: public wxDialog
{
	public:

		AboutDialog(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~AboutDialog();

		//(*Declarations(AboutDialog)
		wxStaticText* libreScribeVersionLabel;
		wxStaticText* StaticText1;
		wxStaticLine* StaticLine1;
		wxStaticBitmap* libreScribeLogoBitmap;
		//*)

	protected:

		//(*Identifiers(AboutDialog)
		static const long idLogoBitmap;
		static const long idLibreScribeVersionLabel;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXT1;
		//*)

	private:

		//(*Handlers(AboutDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
