#ifndef GUIFRAME_H
#define GUIFRAME_H
#include "DeviceInformation.h"
#include "Smartpen.h"
#ifndef WX_PRECOMP
	//(*HeadersPCH(GUIFrame)
	#include <wx/toolbar.h>
	#include <wx/sizer.h>
	#include <wx/listctrl.h>
	#include <wx/menu.h>
	#include <wx/panel.h>
	#include <wx/statusbr.h>
	#include <wx/frame.h>
	#include <wx/stattext.h>
	#include <wx/statbmp.h>
	//*)
#endif
//(*Headers(GUIFrame)
#include <wx/notebook.h>
#include <wx/treectrl.h>
//*)

class GUIFrame: public wxFrame
{
	public:

		GUIFrame(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~GUIFrame();
        void doRefreshDeviceState();
		//(*Declarations(GUIFrame)
		wxMenuItem* MenuItem2;
		wxMenuItem* MenuItem1;
		wxStaticText* notebookPageName;
		wxMenuBar* menuBar;
		wxStatusBar* statusBar;
		wxMenu* Menu1;
		wxPanel* appTab;
		wxTreeCtrl* pageTree;
		wxListCtrl* appList;
		wxPanel* audioTab;
		wxPanel* pagesTab;
		wxStaticText* notebookPageSize;
		wxToolBarToolBase* ToolBarItem2;
		wxStaticBitmap* pageImage;
		wxNotebook* tabContainer;
		wxMenuItem* MenuItem5;
		wxListCtrl* audioList;
		wxToolBarToolBase* ToolBarItem1;
		wxMenu* fileMenu;
		wxMenuItem* MenuItem6;
		wxMenuItem* MenuItem4;
		wxToolBarToolBase* quitButton;
		wxToolBar* mainToolbar;
		wxMenuItem* MenuItem8;
		//*)

	protected:

        //(*Identifiers(GUIFrame)
        static const long ID_TREECTRL1;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_STATICBITMAP1;
        static const long ID_PANEL1;
        static const long ID_LISTCTRL1;
        static const long ID_PANEL2;
        static const long ID_LISTCTRL2;
        static const long ID_PANEL3;
        static const long ID_NOTEBOOK1;
        static const long idMenuFilePrint;
        static const long idMenuFileDeletePages;
        static const long idMenuFileArchiveNotebook;
        static const long idMenuFileDeleteNotebok;
        static const long ID_MENUITEM7;
        static const long idAboutMenuItem;
        static const long idToolbarRefresh;
        static const long idToolbarInfo;
        static const long idToolbarQuit;
        static const long ID_TOOLBAR1;
        static const long ID_STATUSBAR1;
        //*)

	private:

		//(*Handlers(GUIFrame)
		void OnRefresh(wxCommandEvent& event);
		void OnInfo(wxCommandEvent& event);
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
        uint16_t refreshDeviceState();
};

#endif
