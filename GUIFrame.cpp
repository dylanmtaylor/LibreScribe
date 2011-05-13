#include "wx_pch.h"
#include "GUIFrame.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(GUIFrame)
	#include <wx/string.h>
	#include <wx/intl.h>
	//*)
#endif
//(*InternalHeaders(GUIFrame)
#include <wx/font.h>
#include <wx/bitmap.h>
#include <wx/image.h>
//*)

//(*IdInit(GUIFrame)
const long GUIFrame::ID_TREECTRL1 = wxNewId();
const long GUIFrame::ID_STATICTEXT1 = wxNewId();
const long GUIFrame::ID_STATICTEXT2 = wxNewId();
const long GUIFrame::ID_STATICBITMAP1 = wxNewId();
const long GUIFrame::ID_PANEL1 = wxNewId();
const long GUIFrame::ID_LISTCTRL1 = wxNewId();
const long GUIFrame::ID_PANEL2 = wxNewId();
const long GUIFrame::ID_LISTCTRL2 = wxNewId();
const long GUIFrame::ID_PANEL3 = wxNewId();
const long GUIFrame::ID_NOTEBOOK1 = wxNewId();
const long GUIFrame::idMenuFilePrint = wxNewId();
const long GUIFrame::idMenuFileDeletePages = wxNewId();
const long GUIFrame::idMenuFileArchiveNotebook = wxNewId();
const long GUIFrame::idMenuFileDeleteNotebok = wxNewId();
const long GUIFrame::ID_MENUITEM7 = wxNewId();
const long GUIFrame::idAboutMenuItem = wxNewId();
const long GUIFrame::idToolbarRefresh = wxNewId();
const long GUIFrame::idToolbarInfo = wxNewId();
const long GUIFrame::idToolbarQuit = wxNewId();
const long GUIFrame::ID_TOOLBAR1 = wxNewId();
const long GUIFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(GUIFrame,wxFrame)
	//(*EventTable(GUIFrame)
	//*)
END_EVENT_TABLE()

GUIFrame::GUIFrame(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(GUIFrame)
	wxBoxSizer* audioTabContainer;
	wxFlexGridSizer* pageTabContainer;
	wxBoxSizer* pageViewerContainer;
	wxBoxSizer* contentSizer;
	wxFlexGridSizer* pageBrowser;
	wxGridSizer* notebookToolbar;
	wxBoxSizer* appTabContainer;

	Create(parent, wxID_ANY, _("LibreScribe Smartpen Manager [Alpha]"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	SetClientSize(wxSize(750,550));
	SetMinSize(wxSize(750,550));
	contentSizer = new wxBoxSizer(wxHORIZONTAL);
	tabContainer = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxSize(-1,550), 0, _T("ID_NOTEBOOK1"));
	tabContainer->SetMinSize(wxSize(750,550));
	pagesTab = new wxPanel(tabContainer, ID_PANEL1, wxPoint(13,75), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	pageTabContainer = new wxFlexGridSizer(1, 2, 0, 0);
	pageTree = new wxTreeCtrl(pagesTab, ID_TREECTRL1, wxDefaultPosition, wxSize(255,-1), wxTR_NO_BUTTONS|wxTR_NO_LINES|wxTR_FULL_ROW_HIGHLIGHT|wxSUNKEN_BORDER, wxDefaultValidator, _T("ID_TREECTRL1"));
	pageTree->SetMinSize(wxSize(255,-1));
	pageTree->SetMaxSize(wxSize(255,-1));
	pageTabContainer->Add(pageTree, 1, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	pageBrowser = new wxFlexGridSizer(0, 1, 0, 0);
	notebookToolbar = new wxGridSizer(1, 2, 0, 0);
	notebookPageSize = new wxStaticText(pagesTab, ID_STATICTEXT1, _("A5"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	wxFont notebookPageSizeFont(12,wxSWISS,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Sans"),wxFONTENCODING_DEFAULT);
	notebookPageSize->SetFont(notebookPageSizeFont);
	notebookToolbar->Add(notebookPageSize, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	notebookPageName = new wxStaticText(pagesTab, ID_STATICTEXT2, _("Tutorial"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	wxFont notebookPageNameFont(11,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
	notebookPageName->SetFont(notebookPageNameFont);
	notebookToolbar->Add(notebookPageName, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	pageBrowser->Add(notebookToolbar, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	pageViewerContainer = new wxBoxSizer(wxHORIZONTAL);
	pageImage = new wxStaticBitmap(pagesTab, ID_STATICBITMAP1, wxBitmap(wxImage(_T("/home/dylan/cpp/LibreScribe/res/page1.png"))), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICBITMAP1"));
	pageViewerContainer->Add(pageImage, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	pageBrowser->Add(pageViewerContainer, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	pageTabContainer->Add(pageBrowser, 1, wxALL|wxEXPAND|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	pagesTab->SetSizer(pageTabContainer);
	pageTabContainer->Fit(pagesTab);
	pageTabContainer->SetSizeHints(pagesTab);
	audioTab = new wxPanel(tabContainer, ID_PANEL2, wxPoint(35,20), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	audioTabContainer = new wxBoxSizer(wxHORIZONTAL);
	audioList = new wxListCtrl(audioTab, ID_LISTCTRL1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_AUTOARRANGE|wxLC_HRULES|wxSUNKEN_BORDER, wxDefaultValidator, _T("ID_LISTCTRL1"));
	audioTabContainer->Add(audioList, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	audioTab->SetSizer(audioTabContainer);
	audioTabContainer->Fit(audioTab);
	audioTabContainer->SetSizeHints(audioTab);
	appTab = new wxPanel(tabContainer, ID_PANEL3, wxPoint(138,16), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	appTabContainer = new wxBoxSizer(wxHORIZONTAL);
	appList = new wxListCtrl(appTab, ID_LISTCTRL2, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_AUTOARRANGE|wxLC_HRULES|wxSUNKEN_BORDER, wxDefaultValidator, _T("ID_LISTCTRL2"));
	appTabContainer->Add(appList, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	appTab->SetSizer(appTabContainer);
	appTabContainer->Fit(appTab);
	appTabContainer->SetSizeHints(appTab);
	tabContainer->AddPage(pagesTab, _("Pages"), false);
	tabContainer->AddPage(audioTab, _("Audio"), false);
	tabContainer->AddPage(appTab, _("Applications"), false);
	contentSizer->Add(tabContainer, 1, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	SetSizer(contentSizer);
	menuBar = new wxMenuBar();
	fileMenu = new wxMenu();
	MenuItem2 = new wxMenuItem(fileMenu, idMenuFilePrint, _("&Print\tCtrl+P"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append(MenuItem2);
	MenuItem2->Enable(false);
	fileMenu->AppendSeparator();
	MenuItem4 = new wxMenuItem(fileMenu, idMenuFileDeletePages, _("Delete &Page(s)\tCtrl+D"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append(MenuItem4);
	MenuItem4->Enable(false);
	MenuItem5 = new wxMenuItem(fileMenu, idMenuFileArchiveNotebook, _("&Archive Notebook\tShift+Ctrl+A"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append(MenuItem5);
	MenuItem5->Enable(false);
	MenuItem6 = new wxMenuItem(fileMenu, idMenuFileDeleteNotebok, _("&Delete Notebook\tShift+Ctrl+D"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append(MenuItem6);
	MenuItem6->Enable(false);
	fileMenu->AppendSeparator();
	MenuItem8 = new wxMenuItem(fileMenu, ID_MENUITEM7, _("&Quit\tCtrl+Q"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append(MenuItem8);
	menuBar->Append(fileMenu, _("&File"));
	Menu1 = new wxMenu();
	MenuItem1 = new wxMenuItem(Menu1, idAboutMenuItem, _("&About\tF1"), wxEmptyString, wxITEM_NORMAL);
	Menu1->Append(MenuItem1);
	menuBar->Append(Menu1, _("&Help"));
	SetMenuBar(menuBar);
	mainToolbar = new wxToolBar(this, ID_TOOLBAR1, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER, _T("ID_TOOLBAR1"));
	ToolBarItem1 = mainToolbar->AddTool(idToolbarRefresh, _("Refresh"), wxBitmap(wxImage(_T("/home/dylan/cpp/LibreScribe/res/view-refresh.png"))), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
	ToolBarItem2 = mainToolbar->AddTool(idToolbarInfo, _("Information"), wxBitmap(wxImage(_T("/home/dylan/cpp/LibreScribe/res/emblem-system.png"))), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
	quitButton = mainToolbar->AddTool(idToolbarQuit, _("Quit"), wxBitmap(wxImage(_T("/home/dylan/cpp/LibreScribe/res/process-stop.png"))), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString);
	mainToolbar->Realize();
	SetToolBar(mainToolbar);
	statusBar = new wxStatusBar(this, ID_STATUSBAR1, wxST_SIZEGRIP, _T("ID_STATUSBAR1"));
	int __wxStatusBarWidths_1[2] = { -10, -10 };
	int __wxStatusBarStyles_1[2] = { wxSB_NORMAL, wxSB_NORMAL };
	statusBar->SetFieldsCount(2,__wxStatusBarWidths_1);
	statusBar->SetStatusStyles(2,__wxStatusBarStyles_1);
	SetStatusBar(statusBar);
	contentSizer->SetSizeHints(this);
	Center();
	//*)
}

GUIFrame::~GUIFrame()
{
	//(*Destroy(GUIFrame)
	//*)
}
