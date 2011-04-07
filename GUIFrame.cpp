///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIFrame.h"

///////////////////////////////////////////////////////////////////////////

GUIFrame::GUIFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 750,550 ), wxDefaultSize );
	
	mbar = new wxMenuBar( 0 );
	fileMenu = new wxMenu();
	wxMenuItem* menuFilePrint;
	menuFilePrint = new wxMenuItem( fileMenu, idMenuPrint, wxString( wxT("Print") ) + wxT('\t') + wxT("CTRL+P"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( menuFilePrint );
	menuFilePrint->Enable( false );
	
	wxMenuItem* m_separator1;
	m_separator1 = fileMenu->AppendSeparator();
	
	wxMenuItem* menuFileDeletePages;
	menuFileDeletePages = new wxMenuItem( fileMenu, idMenuDelete, wxString( wxT("Delete Page(s)") ) + wxT('\t') + wxT("CTRL-D"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( menuFileDeletePages );
	menuFileDeletePages->Enable( false );
	
	wxMenuItem* menuFileArchiveNotebook;
	menuFileArchiveNotebook = new wxMenuItem( fileMenu, idMenuDelete, wxString( wxT("Archive Notebook") ) + wxT('\t') + wxT("CTRL-SHIFT-A"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( menuFileArchiveNotebook );
	menuFileArchiveNotebook->Enable( false );
	
	wxMenuItem* menuFileDeleteNotebook;
	menuFileDeleteNotebook = new wxMenuItem( fileMenu, idMenuDelete, wxString( wxT("Delete Notebook") ) + wxT('\t') + wxT("CTRL-SHIFT-D"), wxEmptyString, wxITEM_NORMAL );
	fileMenu->Append( menuFileDeleteNotebook );
	menuFileDeleteNotebook->Enable( false );
	
	wxMenuItem* m_separator3;
	m_separator3 = fileMenu->AppendSeparator();
	
	wxMenuItem* menuFileQuit;
	menuFileQuit = new wxMenuItem( fileMenu, idMenuQuit, wxString( wxT("&Quit") ) + wxT('\t') + wxT("CTRL-Q"), wxT("Quit the application"), wxITEM_NORMAL );
	fileMenu->Append( menuFileQuit );
	
	mbar->Append( fileMenu, wxT("&File") );
	
	helpMenu = new wxMenu();
	wxMenuItem* menuHelpAbout;
	menuHelpAbout = new wxMenuItem( helpMenu, idMenuAbout, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxT("Show info about this application"), wxITEM_NORMAL );
	helpMenu->Append( menuHelpAbout );
	
	mbar->Append( helpMenu, wxT("&Help") );
	
	this->SetMenuBar( mbar );
	
	m_toolBar1 = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY ); 
	m_toolBar1->AddTool( idToolbarRefresh, wxT("refresh"), wxBitmap( wxT("res/view-refresh.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Refresh Connection"), wxT("Click this to scan for a compatible smartpen device.") );
	m_toolBar1->AddTool( idToolbarInfo, wxT("info"), wxBitmap( wxT("res/emblem-system.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Device Status"), wxEmptyString );
	m_toolBar1->AddTool( idToolbarQuit, wxT("quit"), wxBitmap( wxT("res/process-stop.png"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Quit Smartpen Manager"), wxEmptyString );
	m_toolBar1->AddSeparator();
	m_toolBar1->Realize();
	
	statusBar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );
	wxBoxSizer* contentSizer;
	contentSizer = new wxBoxSizer( wxVERTICAL );
	
	tabContainer = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	tabContainer->Enable( false );
	
	pagesTab = new wxPanel( tabContainer, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* pageTabContainer;
	pageTabContainer = new wxFlexGridSizer( 1, 2, 0, 0 );
	pageTabContainer->SetFlexibleDirection( wxBOTH );
	pageTabContainer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	pageTree = new wxTreeCtrl( pagesTab, wxID_ANY, wxDefaultPosition, wxSize( 225,420 ), wxTR_DEFAULT_STYLE|wxSIMPLE_BORDER );
	pageTree->SetMinSize( wxSize( 225,420 ) );
	pageTree->SetMaxSize( wxSize( 225,-1 ) );
	
	pageTabContainer->Add( pageTree, 0, wxALL, 5 );
	
	wxFlexGridSizer* pageBrowser;
	pageBrowser = new wxFlexGridSizer( 2, 2, 0, 0 );
	pageBrowser->SetFlexibleDirection( wxBOTH );
	pageBrowser->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxGridSizer* notebookToolbar;
	notebookToolbar = new wxGridSizer( 1, 2, 0, 0 );
	
	notebookPageSize = new wxStaticText( pagesTab, wxID_ANY, wxT("A5"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	notebookPageSize->Wrap( -1 );
	notebookPageSize->SetFont( wxFont( 12, 74, 90, 92, false, wxT("Sans") ) );
	
	notebookToolbar->Add( notebookPageSize, 0, wxALL, 5 );
	
	notebookName = new wxStaticText( pagesTab, wxID_ANY, wxT("Tutorial"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	notebookName->Wrap( -1 );
	notebookToolbar->Add( notebookName, 0, wxALL, 5 );
	
	pageBrowser->Add( notebookToolbar, 1, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	
	pageBrowser->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* pageViewerContainer;
	pageViewerContainer = new wxBoxSizer( wxVERTICAL );
	
	pageImage = new wxStaticBitmap( pagesTab, wxID_ANY, wxBitmap( wxT("res/page1.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	pageViewerContainer->Add( pageImage, 0, wxALL, 5 );
	
	pageBrowser->Add( pageViewerContainer, 1, wxEXPAND, 5 );
	
	pageTabContainer->Add( pageBrowser, 1, wxEXPAND, 5 );
	
	pagesTab->SetSizer( pageTabContainer );
	pagesTab->Layout();
	pageTabContainer->Fit( pagesTab );
	tabContainer->AddPage( pagesTab, wxT("Pages"), true );
	audioTab = new wxPanel( tabContainer, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	audioTab->Enable( false );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_grid2 = new wxGrid( audioTab, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_grid2->CreateGrid( 5, 5 );
	m_grid2->EnableEditing( false );
	m_grid2->EnableGridLines( true );
	m_grid2->EnableDragGridSize( false );
	m_grid2->SetMargins( 0, 0 );
	
	// Columns
	m_grid2->AutoSizeColumns();
	m_grid2->EnableDragColMove( false );
	m_grid2->EnableDragColSize( true );
	m_grid2->SetColLabelSize( 30 );
	m_grid2->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_grid2->EnableDragRowSize( true );
	m_grid2->SetRowLabelSize( 0 );
	m_grid2->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_grid2->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer7->Add( m_grid2, 0, wxALL|wxEXPAND, 5 );
	
	audioTab->SetSizer( bSizer7 );
	audioTab->Layout();
	bSizer7->Fit( audioTab );
	tabContainer->AddPage( audioTab, wxT("Audio"), false );
	applicationsTab = new wxPanel( tabContainer, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	applicationsTab->Enable( false );
	
	tabContainer->AddPage( applicationsTab, wxT("Applications"), false );
	
	contentSizer->Add( tabContainer, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( contentSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Connect( menuFileQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Connect( menuHelpAbout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	this->Connect( idToolbarRefresh, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GUIFrame::OnRefresh ) );
	this->Connect( idToolbarInfo, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GUIFrame::OnInfo ) );
	this->Connect( idToolbarQuit, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GUIFrame::OnQuit ) );
}

GUIFrame::~GUIFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIFrame::OnClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnQuit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GUIFrame::OnAbout ) );
	this->Disconnect( idToolbarRefresh, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GUIFrame::OnRefresh ) );
	this->Disconnect( idToolbarInfo, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GUIFrame::OnInfo ) );
	this->Disconnect( idToolbarQuit, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GUIFrame::OnQuit ) );
}

DeviceInfo::DeviceInfo( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* mainSizer;
	mainSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
	mainSizer->SetFlexibleDirection( wxBOTH );
	mainSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	infoIcon = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("res/dialog-information-128.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	mainSizer->Add( infoIcon, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* dialogSizer;
	dialogSizer = new wxBoxSizer( wxVERTICAL );
	
	deviceType = new wxStaticText( this, wxID_ANY, wxT("LightScribe Pulse(TM) Smartpen"), wxDefaultPosition, wxDefaultSize, 0 );
	deviceType->Wrap( -1 );
	deviceType->SetFont( wxFont( 13, 70, 90, 92, false, wxEmptyString ) );
	
	dialogSizer->Add( deviceType, 0, wxALIGN_LEFT|wxALL, 5 );
	
	wxFlexGridSizer* informationSizer;
	informationSizer = new wxFlexGridSizer( 2, 2, 5, 15 );
	informationSizer->SetFlexibleDirection( wxVERTICAL );
	informationSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	nameLabel = new wxStaticText( this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	nameLabel->Wrap( -1 );
	informationSizer->Add( nameLabel, 0, wxALL, 5 );
	
	deviceName = new wxStaticText( this, wxID_ANY, wxT("Dylan Taylor's Smartpen"), wxDefaultPosition, wxDefaultSize, 0 );
	deviceName->Wrap( -1 );
	informationSizer->Add( deviceName, 0, wxALL, 5 );
	
	batteryLabel = new wxStaticText( this, wxID_ANY, wxT("Battery:"), wxDefaultPosition, wxDefaultSize, 0 );
	batteryLabel->Wrap( -1 );
	informationSizer->Add( batteryLabel, 0, wxALL, 5 );
	
	batteryGauge = new wxGauge( this, wxID_ANY, 100, wxPoint( -1,-1 ), wxSize( 250,25 ), wxGA_HORIZONTAL );
	batteryGauge->SetValue( 50 ); 
	batteryGauge->SetFont( wxFont( 11, 74, 90, 90, false, wxT("Sans") ) );
	batteryGauge->SetMinSize( wxSize( 250,25 ) );
	batteryGauge->SetMaxSize( wxSize( 250,25 ) );
	
	informationSizer->Add( batteryGauge, 0, wxALL, 5 );
	
	storageLabel = new wxStaticText( this, wxID_ANY, wxT("Storage:"), wxDefaultPosition, wxDefaultSize, 0 );
	storageLabel->Wrap( -1 );
	informationSizer->Add( storageLabel, 0, wxALL, 5 );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("1.55GB of 2.13GB remaining"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	informationSizer->Add( m_staticText8, 0, wxALL, 5 );
	
	dialogSizer->Add( informationSizer, 1, wxALIGN_TOP, 5 );
	
	mainSizer->Add( dialogSizer, 1, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 10 );
	
	this->SetSizer( mainSizer );
	this->Layout();
}

DeviceInfo::~DeviceInfo()
{
}
