#include "wx_pch.h"
#include "AboutDialog.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(AboutDialog)
	#include <wx/string.h>
	#include <wx/intl.h>
	//*)
#endif
//(*InternalHeaders(AboutDialog)
#include <wx/bitmap.h>
#include <wx/image.h>
//*)

//(*IdInit(AboutDialog)
const long AboutDialog::idLogoBitmap = wxNewId();
const long AboutDialog::idLibreScribeVersionLabel = wxNewId();
const long AboutDialog::idAboutDialogSeparator = wxNewId();
const long AboutDialog::idDescriptionText = wxNewId();
//*)

BEGIN_EVENT_TABLE(AboutDialog,wxDialog)
	//(*EventTable(AboutDialog)
	//*)
END_EVENT_TABLE()

AboutDialog::AboutDialog(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(AboutDialog)
	wxFlexGridSizer* primarySizer;
	wxFlexGridSizer* contentSizer;

	Create(parent, wxID_ANY, _("About LibreScribe Smartpen Manager"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxSYSTEM_MENU|wxCLOSE_BOX, _T("wxID_ANY"));
	SetClientSize(wxSize(430,283));
	primarySizer = new wxFlexGridSizer(2, 1, 0, 0);
	libreScribeLogoBitmap = new wxStaticBitmap(this, idLogoBitmap, wxBitmap(wxImage(_T("res/librescribe.png"))), wxDefaultPosition, wxDefaultSize, 0, _T("idLogoBitmap"));
	primarySizer->Add(libreScribeLogoBitmap, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	contentSizer = new wxFlexGridSizer(0, 1, 0, 0);
	libreScribeVersionLabel = new wxStaticText(this, idLibreScribeVersionLabel, _("LibreScribe Smartpen Manager; Version 0.30 Alpha"), wxDefaultPosition, wxDefaultSize, 0, _T("idLibreScribeVersionLabel"));
	contentSizer->Add(libreScribeVersionLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	aboutDialogSeparator = new wxStaticLine(this, idAboutDialogSeparator, wxDefaultPosition, wxSize(400,-1), wxLI_HORIZONTAL, _T("idAboutDialogSeparator"));
	contentSizer->Add(aboutDialogSeparator, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	descriptionText = new wxStaticText(this, idDescriptionText, _("LibreScribe is an effort to create an open source, easy-to-use and feature complete LiveScribe desktop client replacement for the Linux operating system. LibreScribe is written and maintained by Dylan Taylor. A large portion of the \"behind-the-scenes\" code is based off of libsmartpen, by Steven Walter. This project is licensed under version 2 of the GNU General Public License."), wxDefaultPosition, wxSize(390,160), wxALIGN_CENTRE, _T("idDescriptionText"));
	contentSizer->Add(descriptionText, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	primarySizer->Add(contentSizer, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(primarySizer);
	primarySizer->SetSizeHints(this);
	Center();
	//*)
}

AboutDialog::~AboutDialog()
{
	//(*Destroy(AboutDialog)
	//*)
}

