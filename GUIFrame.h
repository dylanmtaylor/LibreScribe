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

#ifndef GUIFRAME_H
#define GUIFRAME_H

#include "DeviceInfo.h"
#include "Smartpen.h"
uint16_t refreshDeviceState();
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
#include <wx/imaglist.h>
#include <wx/thread.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
//(*Headers(GUIFrame)
#include <wx/notebook.h>
#include <wx/treectrl.h>
//*)

class GUIFrame;

struct audioClipInfo {
    wxString name;
    wxString duration;
    wxString date;
    wxString size;
};

struct applicationInfo {
    wxString name;
    wxString version;
    wxString size;
};

class BackgroundMonitor : public wxThread
{
    public:
        BackgroundMonitor(GUIFrame* handler) : wxThread(wxTHREAD_DETACHED) {
            m_pHandler = handler;
        };
        ~BackgroundMonitor();
    protected:
        virtual ExitCode Entry();
        GUIFrame* m_pHandler;
};

class GUIFrame: public wxFrame
{
	public:

		GUIFrame(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~GUIFrame();
        void doRefreshDeviceState();
        void addAudioClipToList(audioClipInfo info);
        void addApplicationToList(applicationInfo info);
        BackgroundMonitor *m_pThread;
        wxCriticalSection m_pThreadCS;    // protects the m_pThread pointer
		//(*Declarations(GUIFrame)
		wxToolBarToolBase* devInfoButton;
		wxStaticText* notebookPageName;
		wxMenuBar* menuBar;
		wxStatusBar* statusBar;
		wxPanel* appTab;
		wxMenuItem* deletePagesMenuItem;
		wxTreeCtrl* pageTree;
		wxListCtrl* appList;
		wxPanel* audioTab;
		wxPanel* pagesTab;
		wxStaticText* notebookPageSize;
		wxStaticBitmap* pageImage;
		wxMenuItem* quitMenuItem;
		wxNotebook* tabContainer;
		wxMenuItem* deleteNotebookMenuItem;
		wxListCtrl* audioList;
		wxMenu* fileMenu;
		wxToolBarToolBase* quitButton;
		wxToolBar* mainToolbar;
		wxToolBarToolBase* refreshButton;
		wxMenuItem* aboutMenuItem;
		wxMenuItem* printMenuItem;
		wxMenu* helpMenu;
		wxMenuItem* archiveNotebookMenuItem;
		//*)

	protected:

        //(*Identifiers(GUIFrame)
        static const long ID_TREECTRL1;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_STATICBITMAP1;
        static const long idPagesTab;
        static const long ID_LISTCTRL1;
        static const long idAudioTab;
        static const long ID_LISTCTRL2;
        static const long idAppTab;
        static const long idTabContainer;
        static const long idMenuFilePrint;
        static const long idMenuFileDeletePages;
        static const long idMenuFileArchiveNotebook;
        static const long idMenuFileDeleteNotebok;
        static const long idMenuFileQuit;
        static const long idMenuHelpAbout;
        static const long idToolbarRefresh;
        static const long idToolbarInfo;
        static const long idToolbarQuit;
        static const long idMainToolbar;
        static const long idStatusBar;
        //*)

	private:

		//(*Handlers(GUIFrame)
		void OnRefresh(wxCommandEvent& event);
		void OnInfo(wxCommandEvent& event);
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
        uint16_t refreshDeviceState();
        void StartBackgroundMonitor();
        void setupPageHierarchy();
        void setupLists();
        void refreshLists();
        void refreshApplicationList();
        void handleLsp(xmlNode *lsp);
};

#endif
