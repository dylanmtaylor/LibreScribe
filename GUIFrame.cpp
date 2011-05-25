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
    setupPageHierarchy();
    setupLists();
#if USE_FAKE_SAMPLE_INFORMATION
    audioClipInfo sampleClipInfo = {_("Sample Audio Clip Info"), _("13:37"), _("11/11/2011 11:11AM"), _("421.8 KiB")};
    addAudioClipToList(sampleClipInfo);
    applicationInfo sampleAppInfo = {_("Sample LiveScribe Application"), _("1.0"), _("1.44 MiB")};
    addApplicationToList(sampleAppInfo);
#endif
    doRefreshDeviceState();
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
    pageTree->AppendItem(root, _("A5 Starter Notebook [2]"), 2, 2);
    pageTree->AppendItem(root, _("Tutorial [2]"), 1, 1);
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
    if (device_handle == NULL || dev == NULL) return;
        refreshApplicationList();
        refreshAudioList();
    } catch(...) {
        wxMessageBox(_("Error: Unable to refresh lists."), _("LibreScribe Smartpen Manager"));
    }
}

void GUIFrame::refreshApplicationList() {
    char* s = smartpen_get_penletlist(device_handle);
    printf("Parsing application list...\n%s\n",s);
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
//    char* s = smartpen_get_paperreplay(handle,0);
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
        applicationInfo thisApp = {wxString((char*)group, wxConvUTF8), wxString((char*)ver, wxConvUTF8), wxString((char*)size, wxConvUTF8)};
        addApplicationToList(thisApp);
    }
}

void GUIFrame::setupLists() {
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
        if (dev->descriptor.idProduct == LS_PULSE) {
            printf("LiveScribe Pulse(TM) Smartpen Detected!\n");
        } else if (dev->descriptor.idProduct == LS_ECHO || dev->descriptor.idProduct == 0x1032) {
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
            if (dev->descriptor.idProduct == LS_PULSE) {
                statusBar->SetStatusText(_("LiveScribe Pulse(TM) Smartpen Detected!"), 1);
                printf("LiveScribe Pulse(TM) Smartpen Detected!\n");
            } else if (dev->descriptor.idProduct == LS_ECHO) {
                statusBar->SetStatusText(_("LiveScribe Echo(TM) Smartpen Detected!"), 1);
                printf("LiveScribe Echo(TM) Smartpen Detected!\n");
            } else {
                statusBar->SetStatusText(_("Unknown LiveScribe Device Detected!"), 1);
                printf("Unknown LiveScribe device detected! Attempting to use this device anyways...\n");
            }
            device_handle = smartpen_connect(dev->descriptor.idVendor, dev->descriptor.idProduct);
        }
        refreshLists();
    } catch(...) {
        printf("Failed to search for your Smartpen\n");
    }
    return;
}

void GUIFrame::OnRefresh(wxCommandEvent& event)
{
    if (dev != NULL) refreshLists();
    if (device_handle == NULL) {
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
