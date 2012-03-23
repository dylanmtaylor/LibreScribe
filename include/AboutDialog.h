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
		wxStaticBitmap* libreScribeLogoBitmap;
		wxStaticText* descriptionText;
		wxStaticLine* aboutDialogSeparator;
		//*)

	protected:

		//(*Identifiers(AboutDialog)
		static const long idLogoBitmap;
		static const long idLibreScribeVersionLabel;
		static const long idAboutDialogSeparator;
		static const long idDescriptionText;
		//*)

	private:

		//(*Handlers(AboutDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
