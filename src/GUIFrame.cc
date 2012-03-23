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

#include "LibreScribe.h"
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
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/bitmap.h>
#include <wx/image.h>
//*)

//(*IdInit(GUIFrame)
const long GUIFrame::idPageTreeCtrl = wxNewId();
const long GUIFrame::idSelectedNotebook = wxNewId();
const long GUIFrame::idNotebookBrowserListCtrl = wxNewId();
const long GUIFrame::idNotebookBrowserPanel = wxNewId();
const long GUIFrame::idPagesTab = wxNewId();
const long GUIFrame::idAudioListCtrl = wxNewId();
const long GUIFrame::idAudioTab = wxNewId();
const long GUIFrame::idApplicatonListCtrl = wxNewId();
const long GUIFrame::idAppTab = wxNewId();
const long GUIFrame::idTabContainer = wxNewId();
const long GUIFrame::idMenuFilePrint = wxNewId();
const long GUIFrame::idMenuFileDeletePages = wxNewId();
const long GUIFrame::idMenuFileArchiveNotebook = wxNewId();
const long GUIFrame::idMenuFileDeleteNotebok = wxNewId();
const long GUIFrame::idMenuFileQuit = wxNewId();
const long GUIFrame::idSelectRenderingColors = wxNewId();
const long GUIFrame::idMenuHelpAbout = wxNewId();
const long GUIFrame::idToolbarRefresh = wxNewId();
const long GUIFrame::idToolbarInfo = wxNewId();
const long GUIFrame::idToolbarQuit = wxNewId();
const long GUIFrame::idMainToolbar = wxNewId();
const long GUIFrame::idStatusBar = wxNewId();
const long GUIFrame::idRootItemMenuInformation = wxNewId();
const long GUIFrame::idRootItemMenuRenameDevice = wxNewId();
const long GUIFrame::idRootItemMenuRefresh = wxNewId();
//*)

struct libusb_device_handle *dev;
libusb_device_descriptor descriptor;
Smartpen* smartpen;
wxTreeItemId root;
notebook* currentNotebook;

BEGIN_EVENT_TABLE(GUIFrame,wxFrame)
	//(*EventTable(GUIFrame)
	//*)
END_EVENT_TABLE()

GUIFrame::GUIFrame(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(GUIFrame)
	wxBoxSizer* audioTabContainer;
	wxBoxSizer* pageViewerContainer;
	wxBoxSizer* contentSizer;
	wxFlexGridSizer* pageBrowser;
	wxGridSizer* notebookToolbar;
	wxBoxSizer* appTabContainer;

	Create(parent, wxID_ANY, _("LibreScribe Smartpen Manager [Alpha]"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	SetClientSize(wxSize(850,550));
	SetMinSize(wxSize(850,550));
	contentSizer = new wxBoxSizer(wxVERTICAL);
	tabContainer = new wxNotebook(this, idTabContainer, wxDefaultPosition, wxSize(-1,550), 0, _T("idTabContainer"));
	tabContainer->SetMinSize(wxSize(800,550));
	pagesTab = new wxSplitterWindow(tabContainer, idPagesTab, wxPoint(13,75), wxDefaultSize, wxSP_3D|wxSP_LIVE_UPDATE, _T("idPagesTab"));
	pagesTab->SetMinSize(wxSize(255,255));
	pagesTab->SetMinimumPaneSize(255);
	pageTree = new wxTreeCtrl(pagesTab, idPageTreeCtrl, wxDefaultPosition, wxSize(320,-1), wxTR_NO_BUTTONS|wxTR_NO_LINES|wxTR_FULL_ROW_HIGHLIGHT|wxSUNKEN_BORDER, wxDefaultValidator, _T("idPageTreeCtrl"));
	pageTree->SetMinSize(wxSize(320,-1));
	notebookBrowserPanel = new wxPanel(pagesTab, idNotebookBrowserPanel, wxPoint(214,59), wxDefaultSize, wxTAB_TRAVERSAL, _T("idNotebookBrowserPanel"));
	notebookBrowserPanel->SetMinSize(wxSize(500,-1));
	notebookBrowserPanel->SetMaxSize(wxSize(500,-1));
	pageBrowser = new wxFlexGridSizer(0, 1, 0, 0);
	notebookToolbar = new wxGridSizer(1, 1, 0, 0);
	selectedNotebookName = new wxStaticText(notebookBrowserPanel, idSelectedNotebook, _("No Notebook Selected"), wxDefaultPosition, wxDefaultSize, 0, _T("idSelectedNotebook"));
	wxFont selectedNotebookNameFont(11,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
	selectedNotebookName->SetFont(selectedNotebookNameFont);
	notebookToolbar->Add(selectedNotebookName, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	pageBrowser->Add(notebookToolbar, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	pageViewerContainer = new wxBoxSizer(wxHORIZONTAL);
	notebookBrowser = new wxListCtrl(notebookBrowserPanel, idNotebookBrowserListCtrl, wxDefaultPosition, wxSize(450,465), wxLC_ICON|wxLC_SINGLE_SEL|wxSUNKEN_BORDER, wxDefaultValidator, _T("idNotebookBrowserListCtrl"));
	notebookBrowser->SetMaxSize(wxSize(-1,-1));
	notebookBrowser->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT));
	notebookBrowser->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));
	pageViewerContainer->Add(notebookBrowser, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	pageBrowser->Add(pageViewerContainer, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	notebookBrowserPanel->SetSizer(pageBrowser);
	pageBrowser->Fit(notebookBrowserPanel);
	pageBrowser->SetSizeHints(notebookBrowserPanel);
	pagesTab->SplitVertically(pageTree, notebookBrowserPanel);
	audioTab = new wxPanel(tabContainer, idAudioTab, wxPoint(35,20), wxDefaultSize, wxTAB_TRAVERSAL, _T("idAudioTab"));
	audioTabContainer = new wxBoxSizer(wxHORIZONTAL);
	audioList = new wxListCtrl(audioTab, idAudioListCtrl, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_AUTOARRANGE|wxLC_HRULES|wxSUNKEN_BORDER, wxDefaultValidator, _T("idAudioListCtrl"));
	audioTabContainer->Add(audioList, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	audioTab->SetSizer(audioTabContainer);
	audioTabContainer->Fit(audioTab);
	audioTabContainer->SetSizeHints(audioTab);
	appTab = new wxPanel(tabContainer, idAppTab, wxPoint(138,16), wxDefaultSize, wxTAB_TRAVERSAL, _T("idAppTab"));
	appTabContainer = new wxBoxSizer(wxHORIZONTAL);
	appList = new wxListCtrl(appTab, idApplicatonListCtrl, wxDefaultPosition, wxDefaultSize, wxLC_REPORT|wxLC_AUTOARRANGE|wxLC_HRULES|wxSUNKEN_BORDER, wxDefaultValidator, _T("idApplicatonListCtrl"));
	appTabContainer->Add(appList, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	appTab->SetSizer(appTabContainer);
	appTabContainer->Fit(appTab);
	appTabContainer->SetSizeHints(appTab);
	tabContainer->AddPage(pagesTab, _("Pages"), false);
	tabContainer->AddPage(audioTab, _("Audio"), false);
	tabContainer->AddPage(appTab, _("Applications"), false);
	contentSizer->Add(tabContainer, 1, wxBOTTOM|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 20);
	SetSizer(contentSizer);
	menuBar = new wxMenuBar();
	fileMenu = new wxMenu();
	printMenuItem = new wxMenuItem(fileMenu, idMenuFilePrint, _("&Print\tCtrl+P"), wxEmptyString, wxITEM_NORMAL);
	fileMenu->Append(printMenuItem);
	printMenuItem->Enable(false);
	fileMenu->AppendSeparator();
	deletePagesMenuItem = new wxMenuItem(fileMenu, idMenuFileDeletePages, _("Delete Pa&ge(s)\tCtrl+D"), wxEmptyString, wxITEM_NORMAL);
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
	preferencesMenu = new wxMenu();
	selectRenderingColorsMenuItem = new wxMenuItem(preferencesMenu, idSelectRenderingColors, _("Select Rendering Colors\tShift+Ctrl+C"), wxEmptyString, wxITEM_NORMAL);
	preferencesMenu->Append(selectRenderingColorsMenuItem);
	menuBar->Append(preferencesMenu, _("&Preferences"));
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
	deviceInformationMenuItem = new wxMenuItem((&rootItemMenu), idRootItemMenuInformation, _("Device &Information"), _("Display information about the connected smartpen"), wxITEM_NORMAL);
	rootItemMenu.Append(deviceInformationMenuItem);
	renameSmartpenMenuItem = new wxMenuItem((&rootItemMenu), idRootItemMenuRenameDevice, _("Re&name Smartpen"), _("Change the name of the connected smartpen"), wxITEM_NORMAL);
	rootItemMenu.Append(renameSmartpenMenuItem);
	refreshConnectionMenuItem = new wxMenuItem((&rootItemMenu), idRootItemMenuRefresh, _("&Refresh Connection"), _("Refresh the connection to the attached smartpen"), wxITEM_NORMAL);
	rootItemMenu.Append(refreshConnectionMenuItem);
	colorDialog = new wxColourDialog(this);
	contentSizer->SetSizeHints(this);
	Center();

	Connect(idPageTreeCtrl,wxEVT_COMMAND_TREE_ITEM_COLLAPSED,(wxObjectEventFunction)&GUIFrame::OnPageTreeItemCollapsed);
	Connect(idPageTreeCtrl,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&GUIFrame::OnPageTreeSelectionChanged);
	Connect(idPageTreeCtrl,wxEVT_COMMAND_TREE_ITEM_MENU,(wxObjectEventFunction)&GUIFrame::OnPageTreeItemMenu);
	Connect(idNotebookBrowserListCtrl,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&GUIFrame::OnNotebookBrowserItemActivated);
	Connect(idApplicatonListCtrl,wxEVT_COMMAND_LIST_COL_CLICK,(wxObjectEventFunction)&GUIFrame::OnApplicationListColumnClick);
	Connect(idMenuFileQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&GUIFrame::OnQuit);
	Connect(idMenuHelpAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&GUIFrame::OnAbout);
	Connect(idToolbarRefresh,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&GUIFrame::OnRefresh);
	Connect(idToolbarInfo,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&GUIFrame::OnInfo);
	Connect(idToolbarQuit,wxEVT_COMMAND_TOOL_CLICKED,(wxObjectEventFunction)&GUIFrame::OnQuit);
	Connect(idRootItemMenuInformation,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&GUIFrame::OnInfo);
	Connect(idRootItemMenuRenameDevice,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&GUIFrame::RenameSmartpen);
	Connect(idRootItemMenuRefresh,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&GUIFrame::OnRefresh);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&GUIFrame::OnClose);
	//*)
	printf("LibreScribe Alpha version 0.30, written by Dylan Taylor\n");
	printf("wxWidgets Version: %d.%d.%d\n",wxMAJOR_VERSION,wxMINOR_VERSION,wxRELEASE_NUMBER);
    //the following coSde makes it so the page tree is automatically fitted to the window.
    wxBoxSizer* pageTreeSizer = new wxBoxSizer( wxVERTICAL );
    pageTreeSizer->Add(pageTree, true, wxEXPAND | wxALL, 5);
    pagesTab->SetSizer(pageTreeSizer);
    browserImages = new wxImageList(32,32,false,0);
    browserImages->Add(wxBitmap(_("res/page-icon-32.png")));
    notebookBrowser->SetImageList(browserImages, wxIMAGE_LIST_NORMAL);
    mkdir("./data", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //create data directory if it doesn't exist
    mkdir("./data/extracted", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //create a directory for extracting archives to if it doesn't exist
    StartBackgroundMonitor();
    doRefreshDeviceState();
//    smartpen->setName("Dylan Taylor's Smartpen"); //for testing only
#if USE_FAKE_SAMPLE_INFORMATION
    audioClipInfo sampleClipInfo = {_("Sample Audio Clip Info"), _("13:37"), _("11/11/2011 11:11AM"), _("421.8 KiB")};
    addAudioClipToList(sampleClipInfo);
    applicationInfo sampleAppInfo = {_("Sample LiveScribe Application"), _("1.0"), _("1.44 MiB")};
    addApplicationToList(sampleAppInfo);
#endif
    //set what colors to render the STF files
    renderingForegroundColor = new wxColor(0,0,0); //black
    renderingBackgroundColor = new wxColor(255,255,255); //white
    printf("fg color: %d,%d,%d\n", (int)renderingForegroundColor->Red(), (int)renderingForegroundColor->Green(), (int)renderingForegroundColor->Blue());
    printf("bg color: %d,%d,%d\n", (int)renderingBackgroundColor->Red(), (int)renderingBackgroundColor->Green(), (int)renderingBackgroundColor->Blue());
//    colorDialog->SetTitle(_("Select A Color"));
//    colorDialog->ShowModal();
}

GUIFrame::~GUIFrame()
{
	//(*Destroy(GUIFrame)
	//*)
}

wxBitmap GUIFrame::ScaleImage(const char* filename) {
    //reference: http://www.dreamincode.net/code/snippet2696.htm
    wxString fname(filename, wxConvUTF8);
    wxBitmap orig = wxBitmap(fname);
    wxImage img = orig.ConvertToImage();
    return wxBitmap(img.Rescale(22,22,wxIMAGE_QUALITY_HIGH));
}

void GUIFrame::setupPageHierarchy() {
    printf("Setting up page hierarchy...\n");
    treeImages = new wxImageList(22,22,false,0);
    treeImages->Add(wxBitmap(_("res/pen-icon.png")));
    treeImages->Add(wxBitmap(_("res/page-icon.png")));
    treeImages->Add(wxBitmap(_("res/notepad-icon.png")));
    treeImages->Add(wxBitmap(_("res/no-pen-icon.png")));
    treeImages->Add(wxBitmap(_("res/pen-refresh.png")));
    pageTree->DeleteAllItems(); //in case we call this method more than once
    notebooks.clear();
    pageTree->SetImageList(treeImages);
    if ((smartpen != NULL) && (dev != NULL)) {
        wxString penName(smartpen->getName(), wxConvUTF8);
        root = pageTree->AddRoot(penName, 4); //use refresh icon while retrieving notebooks/pages from pen
    } else {
        root = pageTree->AddRoot(_("No Smartpen Detected"), 3);
        printf("can't retrieve changelist. no smartpen set. perhaps a device isn't connected?\n");
    }
    pageTree->ExpandAll();
    pageTree->SetIndent(10);
    pageTree->SetSpacing(0);
    pageTree->SelectItem(root,true);
    notebookBrowser->ClearAll(); //clear the notebook page browser of all previous data
    selectedNotebookName->SetLabel(_("No Notebook Selected"));
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

//void GUIFrame::addApplicationToList(applicationInfo info) {
//    appList->InsertItem(0, info.name);
//    appList->SetItem(0, 1, info.version);
//    appList->SetItem(0, 2, info.size);
//}

wxThread::ExitCode RefreshListThread::Entry() {
    printf("background thread started successfully.\n");
    wxString oldStatus = m_pHandler->statusBar->GetStatusText(1);
    wxMutexGuiEnter();
    m_pHandler->statusBar->SetStatusText(_("Refreshing device contents, please wait..."), 1);
    m_pHandler->SetActionAllowed(REFRESH,false); //prevent multiple simultaneous refreshes
    m_pHandler->SetActionAllowed(INFORMATION,false); //opening device info when refreshing causes a seg fault
    m_pHandler->SetActionAllowed(RENAME,false); //trying to rename the device when refreshing causes a seg fault
    wxMutexGuiLeave();
    refreshApplicationList();
    refreshPageHierarchy();
    refreshAudioList();
    wxMutexGuiEnter();
    m_pHandler->statusBar->SetStatusText(oldStatus, 1);
    m_pHandler->SetActionAllowed(REFRESH,true);
    m_pHandler->SetActionAllowed(INFORMATION,true);
    m_pHandler->SetActionAllowed(RENAME,true);
    wxMutexGuiLeave();
}

//This method will clear the lists, set them up again, and fill them with new information.
void GUIFrame::refreshLists() {
    try {
        //first we need to clear and setup the lists
        audioList->ClearAll();
        appList->ClearAll();
        setupLists();
        //now we retrieve the data in the background, so that we don't freeze the interface.
        if ((smartpen != NULL) && (dev != NULL)) { //make sure we have a connection to the smartpen before trying to refresh the lists
            printf("Attempting to start background thread to refresh lists...\n");
            RefreshListThread* refresh_thread = new RefreshListThread(this);
            if (refresh_thread->Create() != wxTHREAD_NO_ERROR) {
                printf("Can't create the thread!\n");
                delete refresh_thread;
                refresh_thread = NULL;
            } else if (refresh_thread->Run() != wxTHREAD_NO_ERROR) {
                printf("Can't create the thread!\n");
                delete refresh_thread;
                refresh_thread = NULL;
            }
        } else {
            printf("It doesn't appear that a smartpen is connected. Sorry!\n");
        }
    } catch(...) {
        wxMessageBox(_("Error: Unable to refresh lists."), _("LibreScribe Smartpen Manager"));
    }
}

void RefreshListThread::refreshApplicationList() {
    if ((dev != NULL) && (smartpen != NULL)) {
        const char* s = smartpen->getPenletList();
        int index = 0;
        printf("Parsing application list...\n%s\n",s);
        printf("strlen of s: %d\n", strlen(s));
        xmlDocPtr doc = xmlParseMemory(s, strlen(s));
        xmlNodePtr cur = xmlDocGetRootElement(doc); //current element should be "xml" at this point.
        if (cur == NULL) { //do nothing if the xml document is empty
            printf("cur is NULL!\n");
            xmlFreeDoc(doc);
        } else { //the xml document is not empty, we should be able to parse this.
            if ((xmlStrcmp(cur->name, (const xmlChar *)"xml")) == 0) { //if the current element's name is 'xml'
                cur = cur->children;
                for (cur = cur; cur; cur = cur->next) {
                    if (cur->type == XML_ELEMENT_NODE) {
                        if ((!xmlStrcmp(cur->name, (const xmlChar *)"lsps"))) { //if the current element's name is 'lsps'
                            xmlNode *lsps = cur->children; //get the children of the 'lsps' element
                            for (lsps = lsps; lsps; lsps = lsps->next) {
                                if (lsps->type == XML_ELEMENT_NODE) {
                                    if ((!xmlStrcmp(lsps->name, (const xmlChar *)"lsp"))) { //if the current element's name is 'lsp'
                                        m_pHandler->handleLsp(lsps,index);
                                    }
                                }
                            }
                        }
                    }
                }
                printf("Done parsing application list!\n");
            }
        }
    }
}

bool GUIFrame::PageHierarchyContains(const wxString query) {
    //reference: http://wiki.wxwidgets.org/WxTreeCtrl
    wxTreeItemIdValue cookie;
    wxTreeItemId item = pageTree->GetFirstChild(root, cookie);
	wxTreeItemId child;

	while(item.IsOk()) {
		wxString sData = pageTree->GetItemText(item);
		if (query.CompareTo(sData) == 0) {
			return true;
		}
		item = pageTree->GetNextChild(root, cookie);
	}
	return false; //item not found
}

std::vector<notebookPage> RefreshListThread::ParsePageData(xmlNodePtr cur) {
    std::vector<notebookPage> pageData;
    printf("parsing page data for notebook...\n");
    if ((xmlStrcmp(cur->name, (const xmlChar *)"lsp")) == 0) {
        xmlNode* pages = cur->children;
        for (pages = pages; pages; pages = pages->next) {
             if (pages->type == XML_ELEMENT_NODE) {
                if ((!xmlStrcmp(pages->name, (const xmlChar *)"page"))) { //if the current element's name is 'page'
                    xmlChar* pageAddress = xmlGetProp(pages, (const xmlChar*)"pageaddress");
                    xmlChar* pgNum = xmlGetProp(pages, (const xmlChar*)"page");
                    int pageNumber = atoi((const char*)pgNum);
                    pageNumber += 1; //pages start with the number 0, similar to elements in an array, etc. therefore, start with 1.
                    printf("adding page %d\n",pageNumber);
                    pageData.push_back(notebookPage{pageNumber, (const char*)pageAddress});
                }
             }
        }
    }
    return pageData;
}

void RefreshListThread::refreshPageHierarchy() {
    printf("Attempting to retrieve changelist...\n");
    const char* changelist = smartpen->getChangeList(0);
    printf("Parsing changelist...\n%s\n",changelist);
    printf("strlen of changelist: %d\n", strlen(changelist));
    xmlDocPtr doc = xmlParseMemory(changelist, strlen(changelist));
    xmlNodePtr cur = xmlDocGetRootElement(doc); //current element should be "xml" at this point.
    if (cur == NULL) { //do nothing if the xml document is empty
        printf("cur is NULL!\n");
        xmlFreeDoc(doc);
    } else { //the xml document is not empty. we should be able to parse this.
        if ((xmlStrcmp(cur->name, (const xmlChar *)"xml")) == 0) { //make sure that the current element's name is 'xml'
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
                                        smartpen->getLspData((char*)guid);
                                        if (!m_pHandler->PageHierarchyContains(wxString((char*)title,wxConvUTF8))) {
                                            wxMutexGuiEnter();
                                            if ((!xmlStrcmp(title, (const xmlChar*)"Tutorial"))) { //if it's the tutorial notebook
                                                //override the default icon with the page icon to have consistend behavior with LS Desktop
                                                m_pHandler->pageTree->AppendItem(root, wxString((char*)title,wxConvUTF8), 1, 1);
                                            } else {
                                                std::string path = "./data/extracted/";
                                                path = path + (char*)guid + "/userdata/icon/active_32x32.png";
                                                if (FILE * file = fopen(path.c_str(), "r")) {
                                                    fclose(file); //the file already exists
                                                    printf("Notebook icon file exists: %s\n",path.c_str());
                                                    int bmpID = m_pHandler->treeImages->Add(m_pHandler->ScaleImage(path.c_str()));
                                                    m_pHandler->pageTree->AppendItem(root, wxString((char*)title,wxConvUTF8), bmpID, bmpID);
                                                } else {
                                                    printf("Notebook icon does not exist at \"%s\". Using default icon.\n",path.c_str());
                                                    m_pHandler->pageTree->AppendItem(root, wxString((char*)title,wxConvUTF8), 2, 2);
                                                }
                                            }
                                            printf("Adding notebook with title \"%s\" to notebooks vector.\n",(char*)title);
                                            m_pHandler->notebooks.push_back(notebook{(char*)title,(char*)guid,ParsePageData(lsps)});
                                            wxMutexGuiLeave();
                                        } else {
                                            printf("Duplicate notebook detected. Not creating new notebook.\n");
                                            for (int n = 0; n < m_pHandler->notebooks.size(); n++) { //search through all of the notebooks
                                                if (strcmp((char*)title,m_pHandler->notebooks[n].title) == 0) { //find the one with the matching title
                                                    std::vector<notebookPage> newPages = ParsePageData(lsps);
                                                    m_pHandler->notebooks[n].notebookPages.insert(m_pHandler->notebooks[n].notebookPages.end(), newPages.begin(), newPages.end());
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            printf("Done retrieving notebooks. The notebook vector now contains:\n");
            for (int i = 0; i < m_pHandler->notebooks.size(); i++) {
                printf("   [%d] \"%s\" (\"%s\")\n", i, m_pHandler->notebooks[i].title, m_pHandler->notebooks[i].guid);
                printf("   Pages stored in the \"notebookPages\" vector of this notebook:\n");
                for (int j = 0; j < m_pHandler->notebooks[i].notebookPages.size(); j++) {
                    printf("      [%d] \"%s\"\n", m_pHandler->notebooks[i].notebookPages[j].pageNumber, m_pHandler->notebooks[i].notebookPages[j].pageAddress);
                }
            }
            wxMutexGuiEnter();
            m_pHandler->pageTree->ExpandAll();
            m_pHandler->pageTree->SetItemImage(root, 0); //set icon to pen icon once the notebooks are done being retrieved
            m_pHandler->pageTree->SortChildren(root); //sort the list of notebooks in ascending alphabetical order
            wxMutexGuiLeave();
            printf("Done parsing change list!\n");
        }
    }
}

void RefreshListThread::refreshAudioList() {
//    char* s = smartpen->getPaperReplay(0);
    printf("Parsing audio list...\n");
}

void GUIFrame::handleLsp(xmlNode *lsp, int& index) {
    //we need to make sure this item isn't system software
    if (xmlStrcmp((xmlGetProp(lsp, (const xmlChar*)"group")), (const xmlChar *)"Livescribe Smartpen Update") != 0) {
        if (xmlStrcmp((xmlGetProp(lsp, (const xmlChar*)"group")), (const xmlChar *)"") != 0) { //if the group name is not blank
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
//            addApplicationToList(thisApp);
            appList->InsertItem(index, thisApp.name);
            appList->SetItem(index, 1, thisApp.version);
            appList->SetItem(index, 2, thisApp.size);
            appList->SetItemData(index, index);
            index += 1;
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
    for (int i = 0; i < int(sizeof(audioColumns)/sizeof(wxString)); i++) {
        audioList->InsertColumn(i, audioColumns[i], wxLIST_FORMAT_LEFT, audio_column_width);
    }
    for (int i = 0; i < int(sizeof(appColumns)/sizeof(wxString)); i++) {
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
    uint16_t productID; //the product id of the device. a dummy value of 0x0000 is returned if dev is NULL.
    printf("Searching for your Smartpen... ");
    if (dev != NULL) libusb_reset_device(dev);
    dev = findSmartpen();
    if (dev == NULL) { //If the smartpen wasn't found the function will have returned NULL
        printf("Sorry! No compatible smartpen device found!\n");
        productID = 0x0000;
    } else {
        printf("detecting smartpen...");
        libusb_get_device_descriptor(libusb_get_device(dev),&descriptor);
        printf("smartpen idProduct: %d\n", descriptor.idProduct);
        if (is_ls_pulse(descriptor.idProduct)) {
            printf("LiveScribe Pulse(TM) Smartpen Detected!\n");
        } else if(is_ls_echo(descriptor.idProduct)) {
            printf("LiveScribe Echo(TM) Smartpen Detected!\n");
        } else {
            printf("Unknown LiveScribe device detected! Attempting to use this device anyways...\n");
        }
        productID = descriptor.idProduct;
    }
    return productID;
}

void GUIFrame::doRefreshDeviceState() {
    printf("Searching for your Smartpen... ");
    statusBar->SetStatusText(_("Searching for a compatible smartpen device..."), 1);
    try {
        if (dev != NULL) libusb_reset_device(dev);
        dev = findSmartpen();
        if (dev == NULL) { //If the smartpen wasn't found the function will have returned NULL
            SetActionAllowed(INFORMATION,false);
            printf("Sorry! No compatible smartpen device found!\n");
            statusBar->SetStatusText(_("Unable to locate a compatible Smartpen device"), 1);
        } else {
            libusb_get_device_descriptor(libusb_get_device(dev),&descriptor);
            SetActionAllowed(INFORMATION,true);
            if (is_ls_pulse(descriptor.idProduct)) {
                statusBar->SetStatusText(_("LiveScribe Pulse(TM) Smartpen Detected!"), 1);
                printf("LiveScribe Pulse(TM) Smartpen Detected!\n");
            } else if (is_ls_echo(descriptor.idProduct)) {
                statusBar->SetStatusText(_("LiveScribe Echo(TM) Smartpen Detected!"), 1);
                printf("LiveScribe Echo(TM) Smartpen Detected!\n");
            } else {
                statusBar->SetStatusText(_("Unknown LiveScribe Device Detected!"), 1);
                printf("Unknown LiveScribe device detected! Attempting to use this device anyways...\n");
            }
            printf("assigning smartpen. vendor id: %x product id: %x\n", descriptor.idVendor, descriptor.idProduct);
            smartpen = Smartpen::connect(descriptor.idVendor, descriptor.idProduct);
            if (smartpen == NULL) printf("smartpen assignment failure.\n");
        }
        refreshLists();
    } catch(...) {
        printf("Failed to search for your Smartpen\n");
    }
}

void GUIFrame::OnRefresh(wxCommandEvent& event)
{
    if (dev != NULL) {
        refreshLists();
    } else {
        printf("dev is NULL.\n");
    }
    if (smartpen == NULL) {
        printf("smartpen is NULL.\n");
        wxMessageBox(_("A connection to your Smartpen could not be established. Is it already in use?"), _("Smartpen Connection Failure"));
    }
}

void GUIFrame::OnInfo(wxCommandEvent& event)
{
    if (dev == NULL) doRefreshDeviceState();
    if ((smartpen != NULL) && (dev != NULL)) {
        SetActionAllowed(REFRESH,false);
        SetActionAllowed(RENAME,false);
        DeviceInfo d(this, descriptor.idProduct, smartpen);
        printf("attempting to show device information dialog\n");
        d.ShowModal(); //display the information dialog
        printf("dialog was displayed without a problem\n");
        SetActionAllowed(RENAME,true);
    } else {
        wxMessageBox(_("A connection to your Smartpen could not be established. Is it already in use?"), _("Smartpen Connection Failure"));
    }
    SetActionAllowed(REFRESH,true);
}

void GUIFrame::OnQuit(wxCommandEvent& event)
{
    Destroy();
}

void GUIFrame::OnAbout(wxCommandEvent& event)
{
    AboutDialog d(this);
    d.ShowModal();
//    wxMessageBox(_("Written by Dylan Taylor. A large portion of the code is taken from libsmartpen, written by Steven Walter. This is alpha quality software. Use in production environments is NOT recommended."), _("LibreScribe Smartpen Manager"));
}

void GUIFrame::OnClose(wxCloseEvent& event)
{
        wxCriticalSectionLocker enter(m_pThreadCS);

    if (m_pThread) { // does the thread still exist?
        printf("Deleting thread...\n");
        if (m_pThread->Delete() != wxTHREAD_NO_ERROR ) {
            printf("Can't delete the thread!\n");
        }
    }

    Destroy();
}

void GUIFrame::OnPageTreeItemMenu(wxTreeEvent& event)
{
	wxTreeItemId item = event.GetItem();
    printf("page tree item context menu request detected. item id: %d\n", static_cast<int>(item));
	if (item == root) { //the root item is either the smartpen or the placeholder when no pen is connected
        if (dev == NULL) {
            rootItemMenu.Enable(idRootItemMenuRenameDevice,false);
            rootItemMenu.Enable(idRootItemMenuInformation,false);
        }
        PopupMenu(&rootItemMenu);
	}
}

void GUIFrame::RenameSmartpen(wxCommandEvent& event) {
    printf("rename smartpen option chosen.\n");
    wxString currentName(smartpen->getName(),wxConvUTF8);
    wxTextEntryDialog renameDialog(this,_("Enter the new name for your smartpen"),_("Rename Smartpen"),
                                  currentName,wxOK|wxCANCEL);
    renameDialog.ShowModal();
    wxString newName = renameDialog.GetValue();
    if (newName != currentName) {
        printf("The name entered is not the same as the current name. Asking for confirmation...\n");
        std::string desiredName = std::string(newName.mb_str());
        printf("The new name that was requested is \"%s\". Is this correct?\n",desiredName.c_str());
        std::string promptText = "Rename device to \"" + desiredName + "\"?";
        wxMessageDialog confirmationDialog(this,wxString(promptText.c_str(),wxConvUTF8),_("Confirm Rename Operation"),wxYES_NO);
        if (confirmationDialog.ShowModal() == wxID_YES) {
            printf("Request confirmed. Attempting to rename device...\n");
            smartpen->setName((char*)desiredName.c_str());
            printf("returned from setting pen name. resetting device.\n");
            libusb_close(dev);
            dev = findSmartpen();
            printf("Retrieving smartpen name: %s\n", smartpen->getName());
        } else printf("Rename operation cancelled.\n");
    }
}

int wxCALLBACK SortStringItems(long item1, long item2, long sortData) {
//    printf("sorting items... item1: %d, item2: %d, sortData: %d\n",item1,item2,sortData);
    SortingInformation *SortInfo = (SortingInformation*)sortData;
    int column = SortInfo->Column;
    wxListCtrl* ListCtrl = SortInfo->ListCtrl;
    bool SortOrder = SortInfo->SortOrder; // gets sorting order
    long index1 = ListCtrl->FindItem(-1, item1); // gets index of the first item
    long index2 = ListCtrl->FindItem(-1, item2); // gets index of the second item
    wxListItem Item1;
    Item1.SetId(index1); // set the index
    Item1.SetColumn(column); // set the column
    Item1.SetMask(wxLIST_MASK_TEXT); // enable GetText()
    ListCtrl->GetItem(Item1);
    wxString string1 = Item1.GetText();
//    printf("string1 value: \"%s\"\n",(char*)string1.ToUTF8().data());
    wxListItem Item2;
    Item2.SetId(index2); // set the index
    Item2.SetColumn(column); // set the column
    Item2.SetMask(wxLIST_MASK_TEXT); // enable GetText()
    ListCtrl->GetItem(Item2);
    wxString string2 = Item2.GetText();
//    printf("string2 value: \"%s\"\n",(char*)string2.ToUTF8().data());
    int result;
    if (column == 0) { //application name column
        result = string1.Cmp(string2);
        if (!SortOrder) result = result * -1;
    } else { //version or size column
        long num1, num2;
        if(string1.ToLong(&num1, 10) == true) {
            num1 = wxAtol(string1); //string to number
        }
        if(string2.ToLong(&num2, 10) == true) {
            num2 = wxAtol(string2); //string to number
        }
        if((num1 < num2)) {
            result = SortOrder ? -1 : 1;
        } else if((num1 > num2)) {
            result = SortOrder ? 1 : -1;
        } else {
            result = 0;
        }
    }
//    printf("result: %d\n ",result);
    return result;
}

void GUIFrame::OnApplicationListColumnClick(wxListEvent& event) {
    //reference: http://forums.wxwidgets.org/viewtopic.php?t=30245
    SortingInformation SortInfo;
    if(event.GetColumn() == SortInfo.Column) { // user clicked same column as last time, change the sorting order
        SortInfo.SortOrder = SortInfo.SortOrder ? FALSE : TRUE;
    } else { // user clicked new column, sort ascending
        SortInfo.SortOrder = TRUE;
    }
    SortInfo.Column = event.GetColumn(); // set the column
    SortInfo.ListCtrl = (wxListCtrl*)appList; // set the list control pointer
    printf("Application list column click detected. Column: %d Sorting list...\n", event.GetColumn());
    SortingInformation sortInfo;
    appList->SortItems(SortStringItems,(long)&SortInfo);
}

void GUIFrame::xdgOpenFile(const char* path) {
    if (CheckIfFileExists("xdg/xdg-open")) {
        printf("Attempting to open file: %s\n",path);
        std::string cmd = "xdg/xdg-open ";
        cmd = cmd + path;
        system(cmd.c_str());
    } else {
        printf("Critical Error: xdg-open is not present. Cannot open file.\n");
    }
}

std::string GUIFrame::ConvertIntegerToString(int i) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

void GUIFrame::decryptStfFile(const char* filename) {
    Py_Initialize();
    FILE* parsestf = fopen("stf.py", "r");
    int fgRGB[] = {(int)renderingForegroundColor->Red(), (int)renderingForegroundColor->Green(), (int)renderingForegroundColor->Blue()};
    int bgRGB[] = {(int)renderingBackgroundColor->Red(), (int)renderingBackgroundColor->Green(), (int)renderingBackgroundColor->Blue()};
    std::string setFilesAndColors = "stf_file = \"" + (std::string)filename + "\"\n" +
                       "png_file = \"" + (std::string)filename + ".png\"\n" +
                       "fgRed = \"" + ConvertIntegerToString(fgRGB[0]) + "\"\n" +
                       "fgGreen = \"" + ConvertIntegerToString(fgRGB[1]) + "\"\n" +
                       "fgBlue = \"" + ConvertIntegerToString(fgRGB[2]) + "\"\n" +
                       "bgRed = \"" + ConvertIntegerToString(bgRGB[0]) + "\"\n" +
                       "bgGreen = \"" + ConvertIntegerToString(bgRGB[1]) + "\"\n" +
                       "bgBlue = \"" + ConvertIntegerToString(bgRGB[2]) + "\"\n";
    printf("setting python variables...\n%s\n",setFilesAndColors.c_str());
    PyRun_SimpleString(setFilesAndColors.c_str());
    PyRun_SimpleFile(parsestf,"stf.py");
    Py_Finalize();
}

bool GUIFrame::CheckIfFileExists(const char* path) {
    struct stat st;
    return (stat(path,&st) == 0);
}

std::vector<std::string> GUIFrame::GetDirectoryContents(const char* path, const bool ignorePNG) {
    //reference: http://www.daniweb.com/software-development/cpp/code/216812
    std::vector<std::string> files;
    printf("attempting to open \"%s\"...\n",path);
    DIR* dir = opendir(path);
    if (dir) {
        struct dirent *d;
        printf("scanning \"%s\"...\n",path);
        while((d = readdir(dir)) != 0) {
            if(/*d->d_type == DT_DIR && */strcmp(d->d_name,".") != 0 && strcmp(d->d_name,"..") != 0) {
                std::string dname = d->d_name;
                if ((!ignorePNG) || (dname.find(".png") == std::string::npos)) {
                    //we either aren't ignoring png files or the filename doesn't contain ".png"
                    files.push_back(d->d_name);
                    printf("   %s\n",d->d_name);
                }
            }
        }
        closedir(dir);
    } else {
        printf("Error getting contents of directory: %s\n",path);
    }
    return files;
}

void GUIFrame::OnNotebookBrowserItemActivated(wxListEvent& event) {
    if (currentNotebook != NULL) {
        for (std::vector<notebookPage>::iterator it = currentNotebook->notebookPages.begin(); it != currentNotebook->notebookPages.end(); ++it) {
            if (strcmp(event.GetText().mb_str(),GeneratePageString(it->pageNumber).c_str()) == 0) { //this could be done much more efficiently.
                printf("Notebook Page Clicked: %d; Page Address: %s\n", it->pageNumber, it->pageAddress);
                std::string filePath = "data/extracted/";
                filePath = filePath + currentNotebook->guid + "/data/" + it->pageAddress;
                std::vector<std::string> firstDirListing = GetDirectoryContents(filePath.c_str());
                if (firstDirListing.size() > 0) {
                    filePath = filePath + "/" + firstDirListing[0]; //I'm actually not sure how to predict this directory name yet
                    std::vector<std::string> fileList = GetDirectoryContents(filePath.c_str());
                    if (fileList.size() > 0) {
                        filePath = filePath + "/" + fileList[0];
                        printf("File path of page clicked: %s\nDecrypting stf file...\n", filePath.c_str());
                        wxString oldStatus = statusBar->GetStatusText(1);
                        statusBar->SetStatusText(_("Decrypting the STF file, please wait..."), 1);
                        decryptStfFile(filePath.c_str());
                        statusBar->SetStatusText(oldStatus, 1);
                        filePath = filePath + ".png";
                        if (CheckIfFileExists(filePath.c_str())) {
                            printf("STF file decrypted. Path of PNG file: %s\n", filePath.c_str());
                            xdgOpenFile(filePath.c_str());
                        } else {
                            printf("STF file decryption error: file does not exist: %s\n", filePath.c_str());
                            wxMessageBox(_("An error occured while decrypting the STF file."),_("STF Decryption Failure"));
                        }
                    } else {
                        printf("Unable to determine the name of the stf file...\n");
                    }
                }
                break; //exits the for statement prematurely in order to reduce delays
            } /* else {
                printf("Page clicked does not match page with address %s and page number %d.\n",it->pageAddress,it->pageNumber);
            } */
        }
    }
//    wxMessageBox(_("Sorry, this feature is not implemented yet."), event.GetText());
}

void GUIFrame::SetActionAllowed(const int action, bool allow) {
    switch (action) {
        case REFRESH:
            refreshButton->Enable(allow);
            rootItemMenu.Enable(idRootItemMenuRefresh,allow);
            return;
        case INFORMATION:
            rootItemMenu.Enable(idRootItemMenuInformation,allow);
            devInfoButton->Enable(allow);
            return;
        case RENAME:
            rootItemMenu.Enable(idRootItemMenuRenameDevice,allow);
            return;
        default:
            printf("Invalid action specified. Cannot enable/disable.\n");
            return;
    }
}

const char* GUIFrame::GetNotebookGUID(const char* title) {
    for (int n = 0; n < notebooks.size(); n++) {
        if (strcmp(title,notebooks[n].title) == 0) {
            return notebooks[n].guid;
        }
    }
}

std::string GUIFrame::GeneratePageString(int pageNumber) {
    std::stringstream out;
    out << pageNumber;
    return "Page " + out.str();
}

void GUIFrame::OnPageTreeSelectionChanged(wxTreeEvent& event) {
	wxTreeItemId item = event.GetItem();
	wxString itemText = pageTree->GetItemText(item);
	std::string textString = (std::string)itemText.mb_str();
    const char* text = textString.c_str();
    notebookBrowser->ClearAll(); //clear the notebook page browser of all previous data
    printf("page tree selection changed. id: %d, text: \"%s\"\n", item, text);
    if (item == root) { //the root item is either the smartpen or the placeholder when no pen is connected
        selectedNotebookName->SetLabel(_("No Notebook Selected"));
        currentNotebook = NULL;
	} else {
        selectedNotebookName->SetLabel(itemText);
        const char* guid = GetNotebookGUID(text);
        printf("The selected notebook's GUID is \"%s\"\n",guid);
        int index = 0;
        for (int n = 0; n < notebooks.size(); n++) { //search through all of the notebooks
            if (strcmp(text,notebooks[n].title) == 0) { //find the one with the matching title
                currentNotebook = &notebooks[n];
                for (int p = currentNotebook->notebookPages.size() - 1; p > -1 ; p--) { //search through all of the pages
                    int page = currentNotebook->notebookPages[p].pageNumber;
                    printf("[%d] adding icon for page %d (address: \"%s\")\n",p,page,currentNotebook->notebookPages[p].pageAddress);
                    notebookBrowser->InsertItem(index++, wxString(GeneratePageString(page).c_str(),wxConvUTF8), 0);
                }
            }
        }
	}
}

void GUIFrame::OnPageTreeItemCollapsed(wxTreeEvent& event) {
    pageTree->ExpandAll(); //prevent collapsing items in the tree
}
