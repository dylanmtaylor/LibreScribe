#ifndef GUIFRAME_H
#define GUIFRAME_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(GUIFrame)
	#include <wx/toolbar.h>
	#include <wx/sizer.h>
	#include <wx/listctrl.h>
	#include <wx/menu.h>
	#include <wx/panel.h>
	#include <wx/statusbr.h>
	#include <wx/frame.h>
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

		//(*Declarations(GUIFrame)
		wxMenuItem* MenuItem2;
		wxMenuItem* MenuItem1;
		wxMenuBar* menuBar;
		wxStatusBar* statusBar;
		wxMenu* Menu1;
		wxPanel* appTab;
		wxTreeCtrl* pageTree;
		wxListCtrl* appList;
		wxPanel* audioTab;
		wxPanel* pagesTab;
		wxToolBarToolBase* ToolBarItem2;
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
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
