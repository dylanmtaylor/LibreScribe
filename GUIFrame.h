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
#include <vector>
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
	//*)
#endif
#include <wx/imaglist.h>
#include <wx/thread.h>
#include <wx/defs.h>
#include <wx/sysopt.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
//(*Headers(GUIFrame)
#include <wx/notebook.h>
#include <wx/treectrl.h>
//*)

class GUIFrame;
enum {REFRESH, INFORMATION, RENAME};

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

struct notebook {
    const char* title;
    const char* guid;
};

struct notebookPage {
    short pageNumber;
    const char* guid;
};

typedef struct {
    wxListCtrl *ListCtrl;
    int Column;
    bool SortOrder;
} SortingInformation;

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

class RefreshListThread : public wxThread
{
    public:
        RefreshListThread(GUIFrame* handler) : wxThread(wxTHREAD_DETACHED) {
            m_pHandler = handler;
        };
        ~RefreshListThread(){};
    protected:
        void refreshApplicationList();
        void refreshAudioList();
        void refreshPageHierarchy();
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
        void decryptStfFile(char* filename);
        void SetActionAllowed(int action, bool allow);
        wxBitmap ScaleImage(const char* filename);
        bool PageHierarchyContains(const wxString query);
//        void addApplicationToList(applicationInfo info);
        BackgroundMonitor* m_pThread;
        wxCriticalSection m_pThreadCS;    // protects the m_pThread pointer
        void handleLsp(xmlNode* lsp,int& index);
		//(*Declarations(GUIFrame)
		wxToolBarToolBase* devInfoButton;
		wxListCtrl* notebookBrowser;
		wxMenuItem* renameSmartpenMenuItem;
		wxMenuBar* menuBar;
		wxMenuItem* refreshConnectionMenuItem;
		wxStatusBar* statusBar;
		wxPanel* appTab;
		wxMenuItem* deletePagesMenuItem;
		wxTreeCtrl* pageTree;
		wxListCtrl* appList;
		wxPanel* audioTab;
		wxPanel* pagesTab;
		wxStaticText* selectedNotebookName;
		wxMenuItem* quitMenuItem;
		wxNotebook* tabContainer;
		wxMenuItem* deleteNotebookMenuItem;
		wxMenuItem* deviceInformationMenuItem;
		wxListCtrl* audioList;
		wxMenu* fileMenu;
		wxToolBarToolBase* quitButton;
		wxToolBar* mainToolbar;
		wxToolBarToolBase* refreshButton;
		wxMenuItem* aboutMenuItem;
		wxMenuItem* printMenuItem;
		wxMenu rootItemMenu;
		wxMenu* helpMenu;
		wxMenuItem* archiveNotebookMenuItem;
		//*)
        wxImageList* treeImages;
        wxImageList* browserImages;
        std::vector<notebook> notebooks;
        std::vector<notebookPage> notebookPages;
	protected:

        //(*Identifiers(GUIFrame)
        static const long idPageTreeCtrl;
        static const long idSelectedNotebook;
        static const long idNotebookBrowserListCtrl;
        static const long idPagesTab;
        static const long idAudioListCtrl;
        static const long idAudioTab;
        static const long idApplicatonListCtrl;
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
        static const long idRootItemMenuInformation;
        static const long idRootItemMenuRenameDevice;
        static const long idRootItemMenuRefresh;
        //*)

	private:
		//(*Handlers(GUIFrame)
		void OnRefresh(wxCommandEvent& event);
		void OnInfo(wxCommandEvent& event);
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnPageTreeItemMenu(wxTreeEvent& event);
		void RenameSmartpen(wxCommandEvent& event);
		void OntabContainerPageChanged(wxNotebookEvent& event);
		void OnApplicationListColumnClick(wxListEvent& event);
		void OnNotebookBrowserItemActivated(wxListEvent& event);
		void OnPageTreeSelectionChanged(wxTreeEvent& event);
		//*)
        enum {ASCENDING,DESCENDING};
//        wxListCtrlCompare SortStringItems(long item1, long item2, long sortOrder);
		void OnPageTreePopupClick();
		DECLARE_EVENT_TABLE()
        uint16_t refreshDeviceState();
        void StartBackgroundMonitor();
        void setupPageHierarchy();
        void setupLists();
        void refreshLists();
};

#endif
