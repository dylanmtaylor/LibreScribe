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
const long AboutDialog::ID_STATICLINE1 = wxNewId();
const long AboutDialog::ID_STATICTEXT1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(AboutDialog,wxDialog)
	//(*EventTable(AboutDialog)
	//*)
END_EVENT_TABLE()

AboutDialog::AboutDialog(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(AboutDialog)
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* primarySizer;

	Create(parent, wxID_ANY, _("About LibreScribe Smartpen Manager"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	primarySizer = new wxFlexGridSizer(2, 1, 0, 0);
	libreScribeLogoBitmap = new wxStaticBitmap(this, idLogoBitmap, wxBitmap(wxImage(_T("/home/dylan/cpp/LibreScribe/res/librescribe.png"))), wxDefaultPosition, wxDefaultSize, 0, _T("idLogoBitmap"));
	primarySizer->Add(libreScribeLogoBitmap, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	libreScribeVersionLabel = new wxStaticText(this, idLibreScribeVersionLabel, _("LibreScribe SmartPen Manager; Version 0.25 Alpha"), wxDefaultPosition, wxDefaultSize, 0, _T("idLibreScribeVersionLabel"));
	FlexGridSizer1->Add(libreScribeVersionLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(400,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	FlexGridSizer1->Add(StaticLine1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("LibreScribe is an effort to create an open source, easy-to-use and feature complete LiveScribe desktop client replacement for the Linux operating system. LibreScribe is written and maintained by Dylan Taylor. A large portion of the \"behind-the-scenes\" code is based off of libsmartpen, by Steven Walter. This project is licensed under version 2 of the GNU General Public License."), wxDefaultPosition, wxSize(390,100), wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	primarySizer->Add(FlexGridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(primarySizer);
	primarySizer->Fit(this);
	primarySizer->SetSizeHints(this);
	Center();
	//*)
}

AboutDialog::~AboutDialog()
{
	//(*Destroy(AboutDialog)
	//*)
}

