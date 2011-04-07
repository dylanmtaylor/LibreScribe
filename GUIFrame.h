///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUIFrame__
#define __GUIFrame__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/statusbr.h>
#include <wx/treectrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/panel.h>
#include <wx/grid.h>
#include <wx/notebook.h>
#include <wx/frame.h>
#include <wx/gauge.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define idMenuPrint 1000
#define idMenuDelete 1001
#define idMenuQuit 1002
#define idMenuAbout 1003
#define idToolbarRefresh 1004
#define idToolbarInfo 1005
#define idToolbarQuit 1006

///////////////////////////////////////////////////////////////////////////////
/// Class GUIFrame
///////////////////////////////////////////////////////////////////////////////
class GUIFrame : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* mbar;
		wxMenu* fileMenu;
		wxMenu* helpMenu;
		wxToolBar* m_toolBar1;
		wxStatusBar* statusBar;
		wxNotebook* tabContainer;
		wxPanel* pagesTab;
		wxTreeCtrl* pageTree;
		wxStaticText* notebookPageSize;
		wxStaticText* notebookName;
		
		wxStaticBitmap* pageImage;
		wxPanel* audioTab;
		wxGrid* m_grid2;
		wxPanel* applicationsTab;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRefresh( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnInfo( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		GUIFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("LibreScribe Smartpen Manager [Alpha]"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 750,561 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~GUIFrame();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DeviceInfo
///////////////////////////////////////////////////////////////////////////////
class DeviceInfo : public wxDialog 
{
	private:
	
	protected:
		wxStaticBitmap* infoIcon;
		wxStaticText* deviceType;
		wxStaticText* nameLabel;
		wxStaticText* deviceName;
		wxStaticText* batteryLabel;
		wxGauge* batteryGauge;
		wxStaticText* storageLabel;
		wxStaticText* m_staticText8;
	
	public:
		
		DeviceInfo( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Smartpen Device Information"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 585,165 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );
		~DeviceInfo();
	
};

#endif //__GUIFrame__
