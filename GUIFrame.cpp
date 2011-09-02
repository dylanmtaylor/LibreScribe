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
#include "GUIFrame.h"

//#define USE_FAKE_SAMPLE_INFORMATION true
#ifndef WX_PRECOMP
    #include "wx_pch.h"
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
const long GUIFrame::idPagesTab = wxNewId();
const long GUIFrame::ID_LISTCTRL1 = wxNewId();
const long GUIFrame::idAudioTab = wxNewId();
const long GUIFrame::ID_LISTCTRL2 = wxNewId();
const long GUIFrame::idAppTab = wxNewId();
const long GUIFrame::idTabContainer = wxNewId();
const long GUIFrame::idMenuFilePrint = wxNewId();
const long GUIFrame::idMenuFileDeletePages = wxNewId();
const long GUIFrame::idMenuFileArchiveNotebook = wxNewId();
const long GUIFrame::idMenuFileDeleteNotebok = wxNewId();
const long GUIFrame::idMenuFileQuit = wxNewId();
const long GUIFrame::idMenuHelpAbout = wxNewId();
const long GUIFrame::idToolbarRefresh = wxNewId();
const long GUIFrame::idToolbarInfo = wxNewId();
const long GUIFrame::idToolbarQuit = wxNewId();
const long GUIFrame::idMainToolbar = wxNewId();
const long GUIFrame::idStatusBar = wxNewId();
//*)

struct usb_device *dev;
obex_t *device_handle;
wxImageList* treeImages;

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
	tabContainer = new wxNotebook(this, idTabContainer, wxDefaultPosition, wxSize(-1,550), 0, _T("idTabContainer"));
	tabContainer->SetMinSize(wxSize(750,550));
	pagesTab = new wxPanel(tabContainer, idPagesTab, wxPoint(13,75), wxDefaultSize, wxTAB_TRAVERSAL, _T("idPagesTab"));
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
	pageImage = new wxStaticBitmap(pagesTab, ID_STATICBITMAP1, wxBitmap(wxImage(_T("res/page1.png"))), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICBITMAP1"));
	pageViewerContainer->Add(pageImage, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	pageBrowser->Add(pageViewerContainer, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	pageTabContainer->Add(pageBrowser, 1, wxALL|wxEXPAND|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	pagesTab->SetSizer(pageTabContainer);
	pageTabContainer->Fit(pagesTab);
	pageTabContainer->SetSizeHints(pagesTab);
	audioTab = new wxPanel(tabContainer, idAudioTab, wxPoint(35,20), wxDefaultSize, wxTAB_TRAVERSAL, _T("idAudioTab"));
	audioTabContainer = new wxBoxSizer(wxHORIZONTAL);
	audioList = new wxListCtrl(audioTab, ID_LISTCTRL1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_AUTOARRANGE|wxLC_HRULES|wxSUNKEN_BORDER, wxDefaultValidator, _T("ID_LISTCTRL1"));
	audioTabContainer->Add(audioList, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	audioTab->SetSizer(audioTabContainer);
	audioTabContainer->Fit(audioTab);
	audioTabContainer->SetSizeHints(audioTab);
	appTab = new wxPanel(tabContainer, idAppTab, wxPoint(138,16), wxDefaultSize, wxTAB_TRAVERSAL, _T("idAppTab"));
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
	printMenuItem = new wxMenuItem(fileMenu, idMenuFilePrint, _("&Print\tCtrl+P"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append(printMenuItem);
	printMenuItem->Enable(false);
	fileMenu->AppendSeparator();
	deletePagesMenuItem = new wxMenuItem(fileMenu, idMenuFileDeletePages, _("Delete &Page(s)\tCtrl+D"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append(deletePagesMenuItem);
	deletePagesMenuItem->Enable(false);
	archiveNotebookMenuItem = new wxMenuItem(fileMenu, idMenuFileArchiveNotebook, _("&Archive Notebook\tShift+Ctrl+A"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append(archiveNotebookMenuItem);
	archiveNotebookMenuItem->Enable(false);
	deleteNotebookMenuItem = new wxMenuItem(fileMenu, idMenuFileDeleteNotebok, _("&Delete Notebook\tShift+Ctrl+D"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append(deleteNotebookMenuItem);
	deleteNotebookMenuItem->Enable(false);
	fileMenu->AppendSeparator();
	quitMenuItem = new wxMenuItem(fileMenu, idMenuFileQuit, _("&Quit\tCtrl+Q"), _("Quit this application."), wxITEM_NORMAL);
	fileMenu->Append(quitMenuItem);
	menuBar->Append(fileMenu, _("&File"));
	helpMenu = new wxMenu();
	aboutMenuItem = new wxMenuItem(helpMenu, idMenuHelpAbout, _("&About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
	helpMenu->Append(aboutMenuItem);
	menuBar->Append(helpMenu, _("&Help"));
	SetMenuBar(menuBar);
	mainToolbar = new wxToolBar(this, idMainToolbar, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxNO_BORDER, _T("idMainToolbar"));
	refreshButton = mainToolbar->AddTool(idToolbarRefresh, _("Refresh"), wxBitmap(wxImage(_T("res/view-refresh.png"))), wxNullBitmap, wxITEM_NORMAL, _("Refresh Connection Status"), _("Scan for a compatible smartpen device."));
	devInfoButton = mainToolbar->AddTool(idToolbarInfo, _("Information"), wxBitmap(wxImage(_T("res/emblem-system.png"))), wxNullBitmap, wxITEM_NORMAL, _("Device Information"), _("Display connected smartpen device information."));
	quitButton = mainToolbar->AddTool(idToolbarQuit, _("Quit"), wxBitmap(wxImage(_T("res/process-stop.png"))), wxNullBitmap, wxITEM_NORMAL, _("Quit LibreScribe"), _("Quit this application."));
	mainToolbar->Realize();
	SetToolBar(mainToolbar);
	statusBar = new wxStatusBar(this, idStatusBar, wxST_SIZEGRIP|wxCLIP_CHILDREN|wxFULL_REPAINT_ON_RESIZE, _T("idStatusBar"));
	int __wxStatusBarWidths_1[2] = { -10, -10 };
	int __wxStatusBarStyles_1[2] = { wxSB_NORMAL, wxSB_NORMAL };
	statusBar->SetFieldsCount(2,__wxStatusBarWidths_1);
	statusBar->SetStatusStyles(2,__wxStatusBarStyles_1);
	SetStatusBar(statusBar);
	contentSizer->SetSizeHints(this);
	Center();

	Connect(idMenuFileQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&GUIFrame::OnQuit);
	Connect(idMenuHelpAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&GUIFrame::OnAbout);
	Connect(idToolbarRefresh,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&GUIFrame::OnRefresh);
	Connect(idToolbarInfo,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&GUIFrame::OnInfo);
	Connect(idToolbarQuit,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&GUIFrame::OnQuit);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&GUIFrame::OnClose);
	//*)
	printf("LibreScribe Alpha version 0.03, written by Dylan Taylor\n");
    //the following code makes it so the page tree is automatically fitted to the window.
    wxBoxSizer* pageTreeSizer = new wxBoxSizer( wxVERTICAL );
    pageTreeSizer->Add(pageTree, true, wxEXPAND | wxALL, 5);
    pagesTab->SetSizer(pageTreeSizer);
    StartBackgroundMonitor();
    doRefreshDeviceState();
#if USE_FAKE_SAMPLE_INFORMATION
    audioClipInfo sampleClipInfo = {_("Sample Audio Clip Info"), _("13:37"), _("11/11/2011 11:11AM"), _("421.8 KiB")};
    addAudioClipToList(sampleClipInfo);
    applicationInfo sampleAppInfo = {_("Sample LiveScribe Application"), _("1.0"), _("1.44 MiB")};
    addApplicationToList(sampleAppInfo);
#endif
}

GUIFrame::~GUIFrame()
{
	//(*Destroy(GUIFrame)
	//*)
}

void GUIFrame::setupPageHierarchy() {
    treeImages = new wxImageList(22,22,false,0);
    treeImages->Add(wxBitmap(_("res/pen-icon.png")));
    treeImages->Add(wxBitmap(_("res/page-icon.png")));
    treeImages->Add(wxBitmap(_("res/notepad-icon.png")));
    pageTree->DeleteAllItems(); //in case we call this method more than once
    pageTree->SetImageList(treeImages);
    wxTreeItemId root = pageTree->AddRoot(_("My LiveScribe Smartpen"), 0);
    printf("Attempting to retrieve changelist...\n");
    char *changelist;
    int rc;

    if (!device_handle) {
        printf("can't retrieve changelist. no device_handle set. perhaps a device isn't connected?\n");
    } else {
        changelist = smartpen_get_changelist(device_handle, 0);
        printf("Parsing changelist...\n%s\n",changelist);
        printf("strlen of changelist: %d\n", strlen(changelist));
        xmlDocPtr doc = xmlParseMemory(changelist, strlen(changelist));
        xmlNodePtr cur = xmlDocGetRootElement(doc); //current element should be "xml" at this point.
        if (cur == NULL) {
            printf("cur is NULL!\n");
            xmlFreeDoc(doc);
            return; //do nothing if the xml document is empty
        }
        if ((xmlStrcmp(cur->name, (const xmlChar *)"xml")) != 0) return; //do nothing if the current element's name is not 'xml'
        cur = cur->children;
        for (cur = cur; cur; cur = cur->next) {
            if (cur->type == XML_ELEMENT_NODE) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *)"changelist"))) { //if the current element's name is 'lsps'
                    xmlNode *lsps = cur->children; //get the children of the 'lsps' element
                    for (lsps = lsps; lsps; lsps = lsps->next) {
                         if (lsps->type == XML_ELEMENT_NODE) {
                            if ((!xmlStrcmp(lsps->name, (const xmlChar *)"lsp"))) { //if the current element's name is 'lsp'
                                xmlChar* title = xmlGetProp(lsps, (const xmlChar*)"title");
                                xmlChar* guid = xmlGetProp(lsps, (const xmlChar*)"guid");
                                if (guid != NULL) {
                                    printf("Notebook detected: %s (%s)\n",title,guid);
                                    pageTree->AppendItem(root, wxString((char*)title,wxConvUTF8), 2, 2);
                                }
                            }
                         }
                    }
                }
            }
        }
        printf("Done parsing change list!\n");
    }
//    pageTree->AppendItem(root, _("A5 Starter Notebook [2]"), 2, 2);
//    pageTree->AppendItem(root, _("Tutorial [2]"), 1, 1);
    pageTree->ExpandAll();
    pageTree->SetIndent(10);
    pageTree->SetSpacing(0);
    //this is a huge work in progress... tons of work to do!
}

const wxString audioColumns[] = {_("Session Name"),
                                 _("Duration"),
                                 _("Recorded"),
                                 _("File Size")};

const wxString appColumns[] = {_("Application Name"),
                                 _("Version"),
                                 _("File Size")};

void GUIFrame::addAudioClipToList(audioClipInfo info) {
    audioList->InsertItem(0, info.name);
    audioList->SetItem(0, 1, info.duration);
    audioList->SetItem(0, 2, info.date);
    audioList->SetItem(0, 3, info.size);
}

void GUIFrame::addApplicationToList(applicationInfo info) {
    appList->InsertItem(0, info.name);
    appList->SetItem(0, 1, info.version);
    appList->SetItem(0, 2, info.size);
}

//This method will clear the lists, set them up again, and fill them with new information.
void GUIFrame::refreshLists() {
    try {
        audioList->ClearAll();
        appList->ClearAll();
        setupLists();
        refreshApplicationList();
        refreshAudioList();
        if (device_handle == NULL || dev == NULL) return;
    } catch(...) {
        wxMessageBox(_("Error: Unable to refresh lists."), _("LibreScribe Smartpen Manager"));
    }
}

void GUIFrame::refreshApplicationList() {
    //for some reason, the code crashes when I retrieve the XML from the device, but entering the xml into the code works
//    char * s = smartpen_get_penletlist(device_handle);
    char * s = "<xml>\n	<lsps>\n		<lsp lowpa=\"0x0008a00000031025\" highpa=\"0x0008a00000031025\" name=\"Test Page\" classname=\"0x66d4de896a2c6c85\" fullpath=\"/penlets/LS_PYON_TestPage_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"375555\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000002d016\" highpa=\"0x0008a0000002d017\" name=\"1-Subject Notebook 5 COVER\" classname=\"0xf618a9f721bc2a48\" fullpath=\"/penlets/LS_S01_02_cvr_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"6420\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000001e026\" highpa=\"0x0008a00000020015\" name=\"1-Subject Notebook 5\" classname=\"0x66d4de89be09c678\" fullpath=\"/penlets/LS_S01_02_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"77637\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000031000\" highpa=\"0x0008a00000031018\" name=\"Printable Notepad 2\" classname=\"0xeda365b3f2b70520\" fullpath=\"/penlets/LS_PYON_Printable_2_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"79281\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000031026\" highpa=\"0x0008a0000003103e\" name=\"Printable Notepad 3\" classname=\"0xeda365b3d6b0c04e\" fullpath=\"/penlets/LS_PYON_Printable_3_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"85405\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000003103f\" highpa=\"0x0008a00000031057\" name=\"Printable Notepad 4\" classname=\"0xeda365b30cbc25d7\" fullpath=\"/penlets/LS_PYON_Printable_4_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"85197\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000022006\" highpa=\"0x0008a00000023061\" name=\"1-Subject Notebook 7\" classname=\"0x66d4de894c103d61\" fullpath=\"/penlets/LS_S03_02_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"76834\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000002d01c\" highpa=\"0x0008a0000002d01d\" name=\"1-Subject Notebook 8 COVER\" classname=\"0x006f07e6201c2427\" fullpath=\"/penlets/LS_S04_02_cvr_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"6421\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000023062\" highpa=\"0x0008a00000025051\" name=\"1-Subject Notebook 8\" classname=\"0x66d4de89eda94437\" fullpath=\"/penlets/LS_S04_02_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"75959\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000002d018\" highpa=\"0x0008a0000002d019\" name=\"1-Subject Notebook 6 COVER\" classname=\"0xd99db0393f67e0ce\" fullpath=\"/penlets/LS_S02_02_cvr_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"6420\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000020016\" highpa=\"0x0008a00000022005\" name=\"1-Subject Notebook 6\" classname=\"0x87e2ad1f6ff6380d\" fullpath=\"/penlets/LS_S02_02_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"77312\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000002d01a\" highpa=\"0x0008a0000002d01b\" name=\"1-Subject Notebook 7 COVER\" classname=\"0x6e1dca856291a5bb\" fullpath=\"/penlets/LS_S03_02_cvr_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"6422\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000001a059\" highpa=\"0x0008a0000001b005\" name=\"Printable Notepad 1\" classname=\"0x29667965a552b1b2\" fullpath=\"/penlets/LS_PYON_Printable_1_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"78995\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000001804a\" highpa=\"0x0008a0000001804a\" name=\"System resource 6\" classname=\"0x30d6e72beedd4518\" fullpath=\"/penlets/LS_NavPlus_SewnInFlap_LabelV3_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"3576\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000002025\" highpa=\"0x0008a00000002025\" name=\"System resource 5\" classname=\"0xcd6cf5ba8b047288\" fullpath=\"/penlets/LS_NavPlus_SewnInFlap_Label_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"3551\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f0000000000\" highpa=\"0x00088f0000001043\" name=\"Black Moroccan Mini Wrap 1\" classname=\"0x563f2dab9479f133\" fullpath=\"/penlets/LS_PB_BM_Mini_1_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"60896\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000031058\" highpa=\"0x0008a00000031059\" name=\"Language Card\" classname=\"0x1ffc7b6be5498b1c\" fullpath=\"/penlets/LS_Language_Card_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"4176\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000001e024\" highpa=\"0x0008a0000001e025\" name=\"Tutorial\" classname=\"0xbc61f02c87e32f9b\" fullpath=\"/penlets/LS_Tutorial_Pages_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"1288373\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000002023\" highpa=\"0x0008a00000002023\" name=\"System resource 4\" classname=\"0x011342c7b378b065\" fullpath=\"/penlets/LS_NavPlus_CradleLabel_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"3513\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f000000600c\" highpa=\"0x00088f000000600d\" name=\"Paperblanks Mini Legal Pages\" classname=\"0x306912764e0ef553\" fullpath=\"/penlets/LS_PB_Mini_LegalPages_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"4094\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00001008010\" highpa=\"0x0008a00001008010\" name=\"Paperblanks Mini Momento Pouch\" classname=\"0x8e6f0513dfc67176\" fullpath=\"/penlets/LS_PB_Mini_Momento_Pouch_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"3482\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088d0000000000\" highpa=\"0x00088d0000000001\" name=\"Paperblanks Status Labels\" classname=\"0xd2adfc83c51e7698\" fullpath=\"/penlets/LS_PB_Status_Labels_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5678\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f0000001044\" highpa=\"0x00088f000000301b\" name=\"Handtooled Mini Wrap 1\" classname=\"0x563f2dab94770520\" fullpath=\"/penlets/LS_PB_HT_Mini_1_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"61840\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f000000600a\" highpa=\"0x00088f000000600b\" name=\"Paperblanks Mini Covers\" classname=\"0x53821ddba187edd8\" fullpath=\"/penlets/LS_PB_Mini_Covers_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5258\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f000000600e\" highpa=\"0x00088f000000600e\" name=\"Paperblanks Mini Cover Flap\" classname=\"0x5eaf61c8153e686e\" fullpath=\"/penlets/LS_PB_Mini_Cover_Flap_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"61073\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000011019\" highpa=\"0x0008a00000011019\" name=\"Interactive controls\" classname=\"0x016d342f3b001ffb\" fullpath=\"/penlets/LS_Sticker_Sheet_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5592\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088d000000001a\" highpa=\"0x00088d0000000065\" name=\"Sticky Notes 3x3 Pad 1\" classname=\"0x7a638594d77cd3f6\" fullpath=\"/penlets/LS_StickyNotes_3x3_1_pen.afd\" version=\"2.7\" buildnumber=\"89\" size=\"157453\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088d0000000066\" highpa=\"0x00088d0000001045\" name=\"Sticky Notes 3x3 Pad 2\" classname=\"0x7a6385944aabd2d4\" fullpath=\"/penlets/LS_StickyNotes_3x3_2_pen.afd\" version=\"2.7\" buildnumber=\"123\" size=\"154501\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000002d02f\" highpa=\"0x0008a0000002d03e\" name=\"Status Labels\" classname=\"0x215dfdd539b2167c\" fullpath=\"/penlets/LS_Status_Labels_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"14107\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000001a051\" highpa=\"0x0008a0000001a054\" name=\"Status Labels Round2\" classname=\"0xacdebcde44a84e15\" fullpath=\"/penlets/LS_Status_Labels_Round2_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"6422\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000001a055\" highpa=\"0x0008a0000001a058\" name=\"Status Labels Round3\" classname=\"0xf7eedb6944a84e15\" fullpath=\"/penlets/LS_Status_Labels_Round3_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"7077\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f000000d044\" highpa=\"0x00088f000000e023\" name=\"Sticky Notes 3x5 Pad 3\" classname=\"0x90cf913ef8980c2d\" fullpath=\"/penlets/LS_StickyNotes_3x5_3_pen.afd\" version=\"2.7\" buildnumber=\"45\" size=\"178162\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000030033\" highpa=\"0x0008a00000030035\" name=\"LS_Tap_to_Win_v2\" classname=\"0x318b28c7e36d846f\" fullpath=\"/penlets/LS_Target_TaptoWin_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"4454\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f000000b04f\" highpa=\"0x00088f000000c006\" name=\"Livescribe Getting Started Guide EFIGS\" classname=\"0x9de36c507de1bd27\" fullpath=\"/penlets/LS_GSG_EFIGS_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"32538\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088d0000001046\" highpa=\"0x00088d0000002025\" name=\"Sticky Notes 3x3 Pad 3\" classname=\"0x7a6385949a0236d2\" fullpath=\"/penlets/LS_StickyNotes_3x3_3_pen.afd\" version=\"2.7\" buildnumber=\"94\" size=\"158694\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f000000c018\" highpa=\"0x00088f000000c063\" name=\"Sticky Notes 3x5 Pad 1\" classname=\"0x90cf913ef4ac27e8\" fullpath=\"/penlets/LS_StickyNotes_3x5_1_pen.afd\" version=\"2.7\" buildnumber=\"27\" size=\"175497\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f000000c064\" highpa=\"0x00088f000000d043\" name=\"Sticky Notes 3x5 Pad 2\" classname=\"0x90cf913eb373b70a\" fullpath=\"/penlets/LS_StickyNotes_3x5_2_pen.afd\" version=\"2.7\" buildnumber=\"36\" size=\"171593\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000030067\" highpa=\"0x0008a00000030068\" name=\"Notebook Common Cover\" classname=\"0xbc61f02cb68c6510\" fullpath=\"/penlets/LS_SERIES_02_cvr_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"7226\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000004032\" highpa=\"0x0008a00000006021\" name=\"1-Subject Notebook 1\" classname=\"0x87e2ad1f71be2fa5\" fullpath=\"/penlets/LS_SA1_01_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"76433\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000011010\" highpa=\"0x0008a00000011011\" name=\"1-Subject Notebook 2 COVER\" classname=\"0xbc61f02c2f8aab7a\" fullpath=\"/penlets/LS_SA2_01_CVR_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"6460\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000009035\" highpa=\"0x0008a0000000b024\" name=\"1-Subject Notebook 2\" classname=\"0x87e2ad1f084d5122\" fullpath=\"/penlets/LS_SA2_01_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"77384\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000000202c\" highpa=\"0x0008a0000000202d\" name=\"Starter Notebook COVER\" classname=\"0xbc61f02c7eff551e\" fullpath=\"/penlets/LS_SA0_01_CVR_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"6408\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000000001b\" highpa=\"0x0008a0000000200a\" name=\"Starter Notebook\" classname=\"0x1878f237fa2426fc\" fullpath=\"/penlets/LS_SA0_01_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"77582\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000000604e\" highpa=\"0x0008a0000000604f\" name=\"1-Subject Notebook 1 COVER\" classname=\"0xbc61f02cd3c6bc22\" fullpath=\"/penlets/LS_SA1_01_CVR_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"6415\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000000d015\" highpa=\"0x0008a0000000f004\" name=\"1-Subject Notebook 4\" classname=\"0x87e2ad1f5bb26ac4\" fullpath=\"/penlets/LS_SA4_01_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"75859\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000011016\" highpa=\"0x0008a00000011017\" name=\"Green Single Subject-Series 1 Cover\" classname=\"0xbc61f02cffbdb133\" fullpath=\"/penlets/LS_SA5_01_CVR_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"6442\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000000f005\" highpa=\"0x0008a00000010060\" name=\"Green Single Subject-Series 1\" classname=\"0x87e2ad1f2ef04867\" fullpath=\"/penlets/LS_SA5_01_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"47259\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000011012\" highpa=\"0x0008a00000011013\" name=\"1-Subject Notebook 3 COVER\" classname=\"0xbc61f02c1d88975e\" fullpath=\"/penlets/LS_SA3_01_CVR_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"6434\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000000b025\" highpa=\"0x0008a0000000d014\" name=\"1-Subject Notebook 3\" classname=\"0x87e2ad1f5de486f4\" fullpath=\"/penlets/LS_SA3_01_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"77817\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000011014\" highpa=\"0x0008a00000011015\" name=\"1-Subject Notebook 4 COVER\" classname=\"0xbc61f02c406ab202\" fullpath=\"/penlets/LS_SA4_01_CVR_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"6437\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000101005a\" highpa=\"0x0008a00001012021\" name=\"1-Subject A5 Notebook 2\" classname=\"0x753d6a33a57ea890\" fullpath=\"/penlets/LS_A5_Notebook_02_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"63243\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000101202a\" highpa=\"0x0008a0000101305d\" name=\"1-Subject A5 Notebook 3\" classname=\"0x753d6a3315a2dacd\" fullpath=\"/penlets/LS_A5_Notebook_03_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"63077\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00001013066\" highpa=\"0x0008a0000101502d\" name=\"1-Subject A5 Notebook 4\" classname=\"0x753d6a3360d58933\" fullpath=\"/penlets/LS_A5_Notebook_04_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"62927\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000100e01e\" highpa=\"0x0008a0000100f051\" name=\"1-Subject A5 Notebook 1\" classname=\"0x753d6a333025f4ce\" fullpath=\"/penlets/LS_A5_Notebook_01_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"62882\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x000891000002c067\" highpa=\"0x000891000002e02e\" name=\"1-Subject A4 Notebook 3\" classname=\"0x0c4b71031176a800\" fullpath=\"/penlets/LS_A4_Notebook_3_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"81485\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x000891000002e037\" highpa=\"0x000891000002f06a\" name=\"1-Subject A4 Notebook 4\" classname=\"0x3d8176db2b0d8edc\" fullpath=\"/penlets/LS_A4_Notebook_4_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"79580\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008910000030007\" highpa=\"0x0008910000030010\" name=\"LS_A4_Notebook_Covers\" classname=\"0x1f2ba33a7816d823\" fullpath=\"/penlets/LS_A4_Notebook_Covers_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"12173\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f000000301c\" highpa=\"0x00088f0000003057\" name=\"Flip Notepad 1\" classname=\"0x3ce96ea355e05d59\" fullpath=\"/penlets/LS_Flip_Notepad_01_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"46826\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f0000004028\" highpa=\"0x00088f0000004063\" name=\"Flip Notepad 2\" classname=\"0x3ce96ea371bb2fa5\" fullpath=\"/penlets/LS_Flip_Notepad_02_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"47517\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f0000004064\" highpa=\"0x00088f0000005033\" name=\"Flip Notepad 3\" classname=\"0x3ce96ea32ef04867\" fullpath=\"/penlets/LS_Flip_Notepad_03_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"47976\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f0000006004\" highpa=\"0x00088f0000006007\" name=\"Flip Notepads Top Sheets\" classname=\"0xd98a736605fa0025\" fullpath=\"/penlets/LS_Flip_Notepads_TopSheets_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"6330\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000100d014\" highpa=\"0x0008a0000100d01d\" name=\"LS_A5_Notebook_Covers\" classname=\"0x7d74493ec16cfa11\" fullpath=\"/penlets/LS_A5_Notebook_Covers_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"10162\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000100c012\" highpa=\"0x0008a0000100d009\" name=\"A5 Starter Notebook\" classname=\"0x0bf11a726d11f3f3\" fullpath=\"/penlets/LS_A5_StarterNotebook_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"66278\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000001101a\" highpa=\"0x0008a0000001101b\" name=\"Demo Card\" classname=\"0xeb9beabe37a13433\" fullpath=\"/penlets/LS_Demo_Card_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"4197\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x000891000002b02b\" highpa=\"0x000891000002c066\" name=\"1-Subject A4 Notebook 2\" classname=\"0x1d320750e28bc250\" fullpath=\"/penlets/LS_A4_Notebook_2_pen.afd\" version=\"2.7\" buildnumber=\"3\" size=\"81267\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000001605a\" highpa=\"0x0008a0000001605b\" name=\"Unlined Journal 2 COVER\" classname=\"0xd5c9048d3ab41bbc\" fullpath=\"/penlets/Journal_B_Blank_Series_1_cvr_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5350\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000001605c\" highpa=\"0x0008a00000018049\" name=\"Unlined Journal 2\" classname=\"0x2f87a14d8e4462f6\" fullpath=\"/penlets/Journal_B_Blank_Series_1_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"67223\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000016056\" highpa=\"0x0008a00000016057\" name=\"Lined Journal 2 COVER\" classname=\"0xd5c9048d9a44d252\" fullpath=\"/penlets/Journal_B_Lined_Series_1_cvr_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5349\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000001101e\" highpa=\"0x0008a0000001300b\" name=\"Lined Journal 1\" classname=\"0x2f87a14df99e19bf\" fullpath=\"/penlets/Journal_A_Lined_Series_1_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"55423\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000016058\" highpa=\"0x0008a00000016059\" name=\"Unlined Journal 1 COVER\" classname=\"0xd5c9048d324268a\" fullpath=\"/penlets/Journal_A_Blank_Series_1_cvr_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5346\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000001300c\" highpa=\"0x0008a00000014065\" name=\"Unlined Journal 1\" classname=\"0x2f87a14de7e570c8\" fullpath=\"/penlets/Journal_A_Blank_Series_1_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"67062\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000016054\" highpa=\"0x0008a00000016055\" name=\"Lined Journal 1 COVER\" classname=\"0xd5c9048dd537cf15\" fullpath=\"/penlets/Journal_A_Lined_Series_1_Cover_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5353\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x000891000001b054\" highpa=\"0x000891000001e03b\" name=\"3-Subject Notebook 4\" classname=\"0x51b9f6cd63800143\" fullpath=\"/penlets/LS_3SUBNTBK_04_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"165750\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f0000008008\" highpa=\"0x00088f000000900e\" name=\"Status Labels Part 2\" classname=\"0xd22d78999ae7a50e\" fullpath=\"/penlets/LS_A4A5_Status_Labels_pen.afd\" version=\"2.7\" buildnumber=\"2\" size=\"25989\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x000891000002905b\" highpa=\"0x000891000002b022\" name=\"1-Subject A4 Notebook 1\" classname=\"0x0c4b7103909b595d\" fullpath=\"/penlets/LS_A4_Notebook_1_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"79720\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008910000019000\" highpa=\"0x000891000001b053\" name=\"3-Subject Notebook 3\" classname=\"0x9d27ee267b9af2e2\" fullpath=\"/penlets/LS_3SUBNTBK_03_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"167647\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000014066\" highpa=\"0x0008a00000016053\" name=\"Lined Journal 2\" classname=\"0x2f87a14df99c5f42\" fullpath=\"/penlets/Journal_B_Lined_Series_1_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"56599\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008910000026060\" highpa=\"0x0008910000029047\" name=\"3-Subject Notebook 1\" classname=\"0x228f96915ef4379c\" fullpath=\"/penlets/LS_3SUBNTBK_01_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"166338\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008910000013030\" highpa=\"0x0008910000016017\" name=\"3-Subject Notebook 2\" classname=\"0xed6976547b9af2e2\" fullpath=\"/penlets/LS_3SUBNTBK_02_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"167227\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f0000005034\" highpa=\"0x00088f0000006003\" name=\"Flip Notepad 4\" classname=\"0x3ce96ea34c103d61\" fullpath=\"/penlets/LS_Flip_Notepad_04_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"47304\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000002b020\" highpa=\"0x0008a0000002d00d\" name=\"Lined Journal 4\" classname=\"0x7f77fc966291a5bb\" fullpath=\"/penlets/LS_JL2_02_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"56165\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00089f0000000004\" highpa=\"0x00089f0000001049\" name=\"Lined Journal 5\" classname=\"0x2f87a14d97f2fab6\" fullpath=\"/penlets/LS_JL5_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"57161\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000002d010\" highpa=\"0x0008a0000002d011\" name=\"Lined Journal 4 COVER\" classname=\"0xe767796b9c17ad4f\" fullpath=\"/penlets/LS_JL2_02_cvr_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5335\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000002d00e\" highpa=\"0x0008a0000002d00f\" name=\"Lined Journal 3 COVER\" classname=\"0xbc61f02c3f67e0ce\" fullpath=\"/penlets/LS_JL1_02_cvr_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5337\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000029032\" highpa=\"0x0008a0000002b01f\" name=\"Lined Journal 3\" classname=\"0x2f87a14d90e3a7ce\" fullpath=\"/penlets/LS_JL1_02_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"56547\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000003004d\" highpa=\"0x0008a0000003004e\" name=\"Journal Common Cover\" classname=\"0xd5c9048d2403aaaa\" fullpath=\"/penlets/LS_JL_CVR_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5660\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f000000900f\" highpa=\"0x00088f0000009010\" name=\"Livescribe Korean Language Card\" classname=\"0x8a673aa5bbd01895\" fullpath=\"/penlets/LS_Korean_LangCard_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5776\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00089f0000004056\" highpa=\"0x00089f000000601b\" name=\"Lined Journal 8\" classname=\"0xf2fbed8dc2e12755\" fullpath=\"/penlets/LS_JL8_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"57383\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00089f000000104a\" highpa=\"0x00089f000000300f\" name=\"Lined Journal 6\" classname=\"0x2f87a14db5a90b86\" fullpath=\"/penlets/LS_JL6_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"57349\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00089f0000003010\" highpa=\"0x00089f0000004055\" name=\"Lined Journal 7\" classname=\"0x2f87a14d08436047\" fullpath=\"/penlets/LS_JL7_INT_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"56754\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000025056\" highpa=\"0x0008a00000027043\" name=\"Unlined Journal 4\" classname=\"0x0d75222a67c75d0b\" fullpath=\"/penlets/LS_JBR2_01_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"66105\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f000000a066\" highpa=\"0x00088f000000a067\" name=\"Livescribe Japanese Language Card\" classname=\"0x8a673aa5bbd01333\" fullpath=\"/penlets/LS_Japanese_LangCard_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5920\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00088f0000006008\" highpa=\"0x00088f0000006009\" name=\"Flip Notepad Control Card\" classname=\"0xdfb2ad7b52b9b3bc\" fullpath=\"/penlets/LS_FLNP_Card_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5400\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x00089f0000000001\" highpa=\"0x00089f0000000001\" name=\"Getting Started Guide\" classname=\"0x3b64d630fde1fc11\" fullpath=\"/penlets/LS_GSG_WebInstall_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"3791\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000025052\" highpa=\"0x0008a00000025053\" name=\"Unlined Journal 3 COVER\" classname=\"0xd5c9048db82edf6a\" fullpath=\"/penlets/LS_JBR1_01_cvr_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5354\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000025054\" highpa=\"0x0008a00000025055\" name=\"Unlined Journal 4 COVER\" classname=\"0xbc61f02cb82edf6a\" fullpath=\"/penlets/LS_JBR2_01_cvr_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"5344\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00000027044\" highpa=\"0x0008a00000029031\" name=\"Unlined Journal 3\" classname=\"0x0ca95b6521dbcc4e\" fullpath=\"/penlets/LS_JBR1_01_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"66646\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008950000000000\" highpa=\"0x0008950000000000\" name=\"Livescribe Korea Getting Started Guide\" classname=\"0x6eb3172286b3a418\" fullpath=\"/penlets/LS_Korea_GSG_pen.afd\" version=\"2.7\" buildnumber=\"0\" size=\"4661\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_FR\" classname=\"com.livescribe.LS_Locale_FR\" fullpath=\"/penlets/LS_Locale_FR.jar\" version=\"2.7\" buildnumber=\"50300\" size=\"915035\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"System resource 1\" classname=\"com.livescribe.runtime.SystemPenlet\" fullpath=\"/penlets/LS_System_Penlet.jar\" version=\"1.4.0\" buildnumber=\"49391\" size=\"1449\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_ES\" classname=\"com.livescribe.LS_Locale_ES\" fullpath=\"/penlets/LS_Locale_ES.jar\" version=\"2.7\" buildnumber=\"50300\" size=\"841918\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_EN\" classname=\"com.livescribe.LS_Locale_EN\" fullpath=\"/penlets/LS_Locale_EN.jar\" version=\"2.7\" buildnumber=\"48632\" size=\"2655499\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Paper Replay\" classname=\"com.livescribe.paperreplay.PaperReplay\" fullpath=\"/penlets/LS_Paper_Replay.jar\" version=\"1.7.0\" buildnumber=\"52394\" size=\"1025569\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Translator Demo\" classname=\"com.livescribe.translator.Translator\" fullpath=\"/penlets/LS_Translator_Demo.jar\" version=\"1.3.0\" buildnumber=\"51679\" size=\"3508410\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Movies\" classname=\"com.livescribe.hotspot.HotSpot\" fullpath=\"/penlets/LS_Movies.jar\" version=\"1.3.1\" buildnumber=\"51679\" size=\"17904252\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_ZH\" classname=\"com.livescribe.LS_Locale_ZH\" fullpath=\"/penlets/LS_Locale_ZH.jar\" version=\"2.7\" buildnumber=\"50300\" size=\"5283939\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"System Resources\" classname=\"com.livescribe.LS_System_Resources\" fullpath=\"/penlets/LS_System_Resources.jar\" version=\"2.7\" buildnumber=\"48637\" size=\"3148237\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_DE\" classname=\"com.livescribe.LS_Locale_DE\" fullpath=\"/penlets/LS_Locale_DE.jar\" version=\"2.7\" buildnumber=\"50300\" size=\"1027837\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Piano\" classname=\"com.livescribe.piano.Piano\" fullpath=\"/penlets/LS_Piano.jar\" version=\"1.3.1\" buildnumber=\"52447\" size=\"5569105\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Scientific Calculator\" classname=\"com.livescribe.fpcalculator.FPCalculator\" fullpath=\"/penlets/LS_Scientific_Calculator.jar\" version=\"1.4.0\" buildnumber=\"51679\" size=\"1531325\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Collator\" classname=\"com.livescribe.collator.CollatorPenlet\" fullpath=\"/penlets/LS_Collator.jar\" version=\"1.0.0\" buildnumber=\"47930\" size=\"905959\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Quick Commands\" classname=\"com.livescribe.activeverb.ActiveVerbPenlet\" fullpath=\"/penlets/LS_Quick_Commands.jar\" version=\"1.3.0\" buildnumber=\"52983\" size=\"85080\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"ICR Resources\" classname=\"com.livescribe.LS_ICR_Resources\" fullpath=\"/penlets/LS_ICR_Resources.jar\" version=\"2.7\" buildnumber=\"45336\" size=\"1413\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Settings\" classname=\"com.livescribe.settings.Settings\" fullpath=\"/penlets/LS_Settings.jar\" version=\"1.5.0\" buildnumber=\"52394\" size=\"7627387\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_JA\" classname=\"com.livescribe.LS_Locale_JA\" fullpath=\"/penlets/LS_Locale_JA.jar\" version=\"2.7\" buildnumber=\"51719\" size=\"5130874\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_IT\" classname=\"com.livescribe.LS_Locale_IT\" fullpath=\"/penlets/LS_Locale_IT.jar\" version=\"2.7\" buildnumber=\"50300\" size=\"898085\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_KO\" classname=\"com.livescribe.LS_Locale_KO\" fullpath=\"/penlets/LS_Locale_KO.jar\" version=\"2.7\" buildnumber=\"51719\" size=\"4098710\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Shortcuts\" classname=\"com.livescribe.shortcuts.Shortcuts\" fullpath=\"/penlets/LS_Shortcuts.jar\" version=\"1.0\" buildnumber=\"52527\" size=\"233257\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_MS\" classname=\"com.livescribe.LS_Locale_MS\" fullpath=\"/penlets/LS_Locale_MS.jar\" version=\"2.7\" buildnumber=\"50300\" size=\"525388\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008910001004043\" highpa=\"0x000891000100600e\" name=\"A4 Grid Notebook 1\" classname=\"0x0541bde63fb5042d\" fullpath=\"/penlets/LS_A4_Grid_Notebook_1_pen.afd\" version=\"2.7\" buildnumber=\"77\" size=\"1277567\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x000891000100600f\" highpa=\"0x0008910001007046\" name=\"A4 Grid Notebook 2\" classname=\"0x0541bde611469c8b\" fullpath=\"/penlets/LS_A4_Grid_Notebook_2_pen.afd\" version=\"2.7\" buildnumber=\"79\" size=\"1279704\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008910001007047\" highpa=\"0x0008910001009012\" name=\"A4 Grid Notebook 3\" classname=\"0x0541bde6d99c57f9\" fullpath=\"/penlets/LS_A4_Grid_Notebook_3_pen.afd\" version=\"2.7\" buildnumber=\"80\" size=\"1280485\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008910001009013\" highpa=\"0x000891000100a04a\" name=\"A4 Grid Notebook 4\" classname=\"0x0541bde6f76fcf5f\" fullpath=\"/penlets/LS_A4_Grid_Notebook_4_pen.afd\" version=\"2.7\" buildnumber=\"78\" size=\"1278450\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000104202f\" highpa=\"0x0008a00001043066\" name=\"A5 Grid Notebook 1\" classname=\"0xcd1e74c1f4b752f0\" fullpath=\"/penlets/LS_A5_Grid_Notebook_1_pen.afd\" version=\"2.7\" buildnumber=\"90\" size=\"1495826\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00001043067\" highpa=\"0x0008a00001045032\" name=\"A5 Grid Notebook 2\" classname=\"0xcd1e74c1e88b0c8c\" fullpath=\"/penlets/LS_A5_Grid_Notebook_2_pen.afd\" version=\"2.7\" buildnumber=\"89\" size=\"1500465\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a00001045033\" highpa=\"0x0008a0000104606a\" name=\"A5 Grid Notebook 3\" classname=\"0xcd1e74c1bc3e97f8\" fullpath=\"/penlets/LS_A5_Grid_Notebook_3_pen.afd\" version=\"2.7\" buildnumber=\"86\" size=\"1500953\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000104606b\" highpa=\"0x0008a00001048036\" name=\"A5 Grid Notebook 4\" classname=\"0xcd1e74c1a002c984\" fullpath=\"/penlets/LS_A5_Grid_Notebook_4_pen.afd\" version=\"2.7\" buildnumber=\"88\" size=\"1498851\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000103c031\" highpa=\"0x0008a0000103c03a\" name=\"Sample Sheets - Blank\" classname=\"0xe55cccb660cac71c\" fullpath=\"/penlets/LS_InBox_Sample_Sheets_Blank_pen.afd\" version=\"2.7\" buildnumber=\"55\" size=\"178824\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000103c063\" highpa=\"0x0008a0000103d000\" name=\"Sample Sheets - Grid\" classname=\"0xe55cccb671b931a6\" fullpath=\"/penlets/LS_InBox_Sample_Sheets_Grid_pen.afd\" version=\"2.7\" buildnumber=\"58\" size=\"189237\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000103d029\" highpa=\"0x0008a0000103d032\" name=\"Sample Sheets - Lined\" classname=\"0xe55cccb607e2f080\" fullpath=\"/penlets/LS_InBox_Sample_Sheets_Lined_pen.afd\" version=\"2.7\" buildnumber=\"57\" size=\"181242\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_NL\" classname=\"com.livescribe.LS_Locale_NL\" fullpath=\"/penlets/LS_Locale_NL.jar\" version=\"2.7\" buildnumber=\"50300\" size=\"977099\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_TR\" classname=\"com.livescribe.LS_Locale_TR\" fullpath=\"/penlets/LS_Locale_TR.jar\" version=\"2.7\" buildnumber=\"50300\" size=\"972570\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"ZPen\" classname=\"zpen.ZPen\" fullpath=\"/penlets/ZPen.jar\" version=\"1.0.0\" buildnumber=\"228\" size=\"195323\" group=\"Zork 1\" groupversion=\"1.0\" category=\"Games\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"dfn-smartpen-ticktacktoe\" classname=\"com.difane.games.ticktacktoe.BasePenlet\" fullpath=\"/penlets/dfn-smartpen-ticktacktoe.jar\" version=\"1.0.0\" buildnumber=\"105\" size=\"649086\" group=\"Tick-Tack-Toe by Difane\" groupversion=\"1.0\" category=\"Games\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Wikipedia DE\" classname=\"com.penlets.wikicommon.de.WikipediaDE\" fullpath=\"/penlets/LS_Wikipedia_DE.jar\" version=\"1.0.0\" buildnumber=\"1590\" size=\"39472648\" group=\"German Wikipedia\" groupversion=\"1.0\" category=\"Education\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Lowest Terms\" classname=\"com.qindesign.penlet.lowestterms.LowestTerms\" fullpath=\"/penlets/QD_Lowest_Terms.jar\" version=\"1.0\" buildnumber=\"65\" size=\"258597\" group=\"Lowest Terms\" groupversion=\"1.0\" category=\"Education\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Guitar_Chords_for_Beginners\" classname=\"com.livescribe.appengine.Guitar_Chords_for_Beginners\" fullpath=\"/penlets/LS_Guitar_Chords_for_Beginners_1.0.9.4.jar\" version=\"1.0.0.NA\" buildnumber=\"0\" size=\"1294547\" group=\"Guitar Chords for Beginners\" groupversion=\"1.0\" category=\"\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Classical_Music_Snippets\" classname=\"com.livescribe.appengine.Classical_Music_Snippets\" fullpath=\"/penlets/LS_Classical_Music_Snippets_1.0.9.7.jar\" version=\"1.0.0.NA\" buildnumber=\"0\" size=\"20691291\" group=\"Classical Music Snippets\" groupversion=\"1.0\" category=\"\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"US_State_Facts\" classname=\"com.livescribe.appengine.US_State_Facts\" fullpath=\"/penlets/LS_US_State_Facts_1.0.9.6.jar\" version=\"1.0.0.NA\" buildnumber=\"0\" size=\"561438\" group=\"U.S. State Facts\" groupversion=\"1.0\" category=\"\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Anniversary_Gifts\" classname=\"com.livescribe.appengine.Anniversary_Gifts\" fullpath=\"/penlets/LS_Anniversary_Gifts_1.0.9.3.jar\" version=\"1.0.0.NA\" buildnumber=\"0\" size=\"94730\" group=\"Anniversary Gifts\" groupversion=\"1.0\" category=\"\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Academy_Award_Winners\" classname=\"com.livescribe.appengine.Academy_Award_Winners\" fullpath=\"/penlets/LS_Academy_Award_Winners_1.0.9.3.jar\" version=\"1.0.0.NA\" buildnumber=\"0\" size=\"405411\" group=\"Academy Award Winners\" groupversion=\"1.0\" category=\"\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"World_Series_Champions\" classname=\"com.livescribe.appengine.World_Series_Champions\" fullpath=\"/penlets/LS_World_Series_Champions_1.0.9.5.jar\" version=\"1.0.0.NA\" buildnumber=\"0\" size=\"251273\" group=\"World Series Champions\" groupversion=\"1.0\" category=\"\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"US_Presidents\" classname=\"com.livescribe.appengine.US_Presidents\" fullpath=\"/penlets/LS_US_Presidents_1.0.9.2.jar\" version=\"1.0.0.NA\" buildnumber=\"0\" size=\"160734\" group=\"U.S. Presidents\" groupversion=\"1.0\" category=\"\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"SpellingMe\" classname=\"com.ripware.apps.smartpen.SpellingMe\" fullpath=\"/penlets/NEWSpellingMe.jar\" version=\"1.0.0\" buildnumber=\"0\" size=\"9110488\" group=\"SpellingMe\" groupversion=\"1.0\" category=\"Education\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"How_to_Say_I_Love_You\" classname=\"com.livescribe.appengine.How_to_Say_I_Love_You\" fullpath=\"/penlets/APP-20022_.jar\" version=\"1.0.0.NA\" buildnumber=\"0\" size=\"71078\" group=\"How to Say 'I Love You'\" groupversion=\"1.0\" category=\"Reference\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Livescribe Send\" classname=\"com.livescribe.connect.send.Send\" fullpath=\"/penlets/LS_Livescribe_Send.jar\" version=\"1.0.0_rNA\" buildnumber=\"0\" size=\"847119\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_AR\" classname=\"com.livescribe.LS_Locale_AR\" fullpath=\"/penlets/LS_Locale_AR.jar\" version=\"2.7\" buildnumber=\"50879\" size=\"1130211\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0000000000000000\" highpa=\"0x0000000000000000\" name=\"Locale_PT\" classname=\"com.livescribe.LS_Locale_PT\" fullpath=\"/penlets/LS_Locale_PT.jar\" version=\"2.7\" buildnumber=\"50300\" size=\"896657\" group=\"Livescribe Smartpen Update\" groupversion=\"2.7.1.166\" category=\"System Software\" locked=\"true\" private=\"false\" installed=\"true\" certificate=\"none\" />\n		<lsp lowpa=\"0x0008a0000101e068\" highpa=\"0x0008a0000101e068\" name=\"Livescribe Connect Dataset\" classname=\"0x66d4de891e4ebae1\" fullpath=\"/penlets/LivescribeConnect_dataset.afd\" version=\"0.0\" buildnumber=\"0\" size=\"24281\" group=\"\" groupversion=\"0.0\" category=\"\" locked=\"false\" private=\"false\" installed=\"true\" certificate=\"none\" />\n	</lsps>\n</xml>";
    printf("Parsing application list...\n%s\n",s);
    printf("strlen of s: %d\n", strlen(s));
    xmlDocPtr doc = xmlParseMemory(s, strlen(s));
    xmlNodePtr cur = xmlDocGetRootElement(doc); //current element should be "xml" at this point.
    if (cur == NULL) {
        printf("cur is NULL!\n");
        xmlFreeDoc(doc);
        return; //do nothing if the xml document is empty
    }
    if ((xmlStrcmp(cur->name, (const xmlChar *)"xml")) != 0) return; //do nothing if the current element's name is not 'xml'
    cur = cur->children;
    for (cur = cur; cur; cur = cur->next) {
        if (cur->type == XML_ELEMENT_NODE) {
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"lsps"))) { //if the current element's name is 'lsps'
                xmlNode *lsps = cur->children; //get the children of the 'lsps' element
                for (lsps = lsps; lsps; lsps = lsps->next) {
                     if (lsps->type == XML_ELEMENT_NODE) {
                        if ((!xmlStrcmp(lsps->name, (const xmlChar *)"lsp"))) { //if the current element's name is 'lsp'
                            handleLsp(lsps);
                        }
                     }
                }
            }
        }
    }
    printf("Done parsing application list!\n");
}

void GUIFrame::refreshAudioList() {
//    char* s = smartpen_get_paperreplay(device_handle,0);
    printf("Parsing audio list...\n");
    return;
}

void GUIFrame::handleLsp(xmlNode *lsp) {
    //we need to make sure this item isn't system software
    if (xmlStrcmp((xmlGetProp(lsp, (const xmlChar*)"group")), (const xmlChar *)"Livescribe Smartpen Update") != 0) {
        printf("Non-system lsp detected:\n");
        xmlChar* name = xmlGetProp(lsp, (const xmlChar*)"name");
        xmlChar* group = xmlGetProp(lsp, (const xmlChar*)"group");
        xmlChar* ver = xmlGetProp(lsp, (const xmlChar*)"groupversion");
        xmlChar* size = xmlGetProp(lsp, (const xmlChar*)"size");
        xmlChar* fullPath = xmlGetProp(lsp, (const xmlChar*)"fullpath");
        printf("\tName: %s\n",name);
        printf("\tGroup: %s\n",group);
        printf("\tVersion: %s\n",ver);
        printf("\tSize: %s\n",size);
        printf("\tFull path: %s\n",fullPath);
        if (name != NULL) {
            applicationInfo thisApp = {wxString((char*)group, wxConvUTF8), wxString((char*)ver, wxConvUTF8), wxString((char*)size, wxConvUTF8)};
            addApplicationToList(thisApp);
        }
    }
}

void GUIFrame::setupLists() {
    setupPageHierarchy();
    const int audio_column_width = 180;
    const int app_column_width = 240;
    //first, set up the list control sizers
    wxBoxSizer* audioSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* appSizer = new wxBoxSizer( wxVERTICAL );
    audioSizer->Add(audioList, true, wxEXPAND | wxALL, 5);
    appSizer->Add(appList, true, wxEXPAND | wxALL, 5);
    audioTab->SetSizer(audioSizer);
    appTab->SetSizer(appSizer);
    for (int i = 0; i < (sizeof(audioColumns)/sizeof(wxString)); i++) {
        audioList->InsertColumn(i, audioColumns[i], wxLIST_FORMAT_LEFT, audio_column_width);
    }
    for (int i = 0; i < (sizeof(appColumns)/sizeof(wxString)); i++) {
        appList->InsertColumn(i, appColumns[i], wxLIST_FORMAT_LEFT, app_column_width);
    }
}

//the following method is based on wxWidgets sample code at http://docs.wxwidgets.org/trunk/classwx_thread.html
void GUIFrame::StartBackgroundMonitor() {
    m_pThread = new BackgroundMonitor(this);

    if ( m_pThread->Create() != wxTHREAD_NO_ERROR )
    {
        printf("Can't create the thread!\n");
        delete m_pThread;
        m_pThread = NULL;
    }
    else
    {
        if (m_pThread->Run() != wxTHREAD_NO_ERROR )
        {
            printf("Can't create the thread!\n");
            delete m_pThread;
            m_pThread = NULL;
        }

        // after the call to wxThread::Run(), the m_pThread pointer is "unsafe":
        // at any moment the thread may cease to exist (because it completes its work).
        // To avoid dangling pointers OnThreadExit() will set m_pThread
        // to NULL when the thread dies.
    }
}

uint16_t GUIFrame::refreshDeviceState() {
    printf("Searching for your Smartpen... ");
    dev = findSmartpen();
    if (dev == NULL) { //If the smartpen wasn't found the function will have returned NULL
        printf("Sorry! No compatible smartpen device found!\n");
        return 0x0000;
    } else {
        printf("detecting smartpen...");
        printf("smartpen idProduct: %s\n", dev->descriptor.idProduct);
        if (is_ls_pulse(dev->descriptor.idProduct)) {
            printf("LiveScribe Pulse(TM) Smartpen Detected!\n");
        } else if(is_ls_echo(dev->descriptor.idProduct)) {
            printf("LiveScribe Echo(TM) Smartpen Detected!\n");
        } else {
            printf("Unknown LiveScribe device detected! Attempting to use this device anyways...\n");
        }
        return dev->descriptor.idProduct;
    }
}

void GUIFrame::doRefreshDeviceState() {
    printf("Searching for your Smartpen... ");
    statusBar->SetStatusText(_("Searching for a compatible smartpen device..."), 1);
    try {
        dev = findSmartpen();
        if (dev == NULL) { //If the smartpen wasn't found the function will have returned NULL
            this->mainToolbar->EnableTool(idToolbarInfo,false);
            printf("Sorry! No compatible smartpen device found!\n");
            statusBar->SetStatusText(_("Unable to locate a compatible Smartpen device"), 1);
        } else {
            this->mainToolbar->EnableTool(idToolbarInfo,true);
            if (is_ls_pulse(dev->descriptor.idProduct)) {
                statusBar->SetStatusText(_("LiveScribe Pulse(TM) Smartpen Detected!"), 1);
                printf("LiveScribe Pulse(TM) Smartpen Detected!\n");
            } else if (is_ls_echo(dev->descriptor.idProduct)) {
                statusBar->SetStatusText(_("LiveScribe Echo(TM) Smartpen Detected!"), 1);
                printf("LiveScribe Echo(TM) Smartpen Detected!\n");
            } else {
                statusBar->SetStatusText(_("Unknown LiveScribe Device Detected!"), 1);
                printf("Unknown LiveScribe device detected! Attempting to use this device anyways...\n");
            }
            printf("assigning device_handle.\n");
            device_handle = smartpen_connect(dev->descriptor.idVendor, dev->descriptor.idProduct);
            if (device_handle == NULL) printf("device_handle assignment failure.\n");
        }
        refreshLists();
    } catch(...) {
        printf("Failed to search for your Smartpen\n");
    }
    return;
}

void GUIFrame::OnRefresh(wxCommandEvent& event)
{
    if (dev != NULL) {
        refreshLists();
    } else {
        printf("dev is NULL.\n");
    }
    if (device_handle == NULL) {
        printf("device_handle is NULL.\n");
        wxMessageBox(_("A connection to your Smartpen could not be established. Is it already in use?"), _("Smartpen Connection Failure"));
        return;
    }
}

void GUIFrame::OnInfo(wxCommandEvent& event)
{
    if (dev == NULL) doRefreshDeviceState();
    if (device_handle != NULL) {
        wxString deviceName("My Smartpen", wxConvUTF8);
        DeviceInfo d(this, deviceName,dev->descriptor.idProduct,device_handle);
        printf("attempting to show device information dialog\n");
        d.ShowModal(); //display the information dialog
        printf("dialog was displayed without a problem\n");
    } else {
        wxMessageBox(_("A connection to your Smartpen could not be established. Is it already in use?"), _("Smartpen Connection Failure"));
    }
}

void GUIFrame::OnQuit(wxCommandEvent& event)
{
    Destroy();
}

void GUIFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(_("Written by Dylan Taylor. A large portion of the code is taken from libsmartpen, written by Steven Walter. This is alpha quality software. Use in production environments is NOT recommended."), _("LibreScribe Smartpen Manager"));
}

void GUIFrame::OnClose(wxCloseEvent& event)
{
        wxCriticalSectionLocker enter(m_pThreadCS);

    if (m_pThread) { // does the thread still exist?
        printf("Deleting thread...\n");
        if (m_pThread->Delete() != wxTHREAD_NO_ERROR ) {
            printf("Can't delete the thread!\n");
        }
    }       // exit from the critical section to give the thread
        // the possibility to enter its destructor
        // (which is guarded with m_pThreadCS critical section!)

//    while (true) {
//        { // was the ~BackgroundMonitor() function executed?
//            wxCriticalSectionLocker enter(m_pThreadCS);
//            if (!m_pThread) break;
//        }
//        // wait for thread completion
//        wxThread::This()->Sleep(1);
//    }

    Destroy();
}
