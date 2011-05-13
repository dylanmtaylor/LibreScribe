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

#ifndef LibreScribe__MAIN_H
#define LibreScribe__MAIN_H
#include <openobex/obex.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <arpa/inet.h>
#include <glib-2.0/glib.h>
#include <libudev.h>
#include "LibreScribe__App.h"
#include "GUIFrame.h"
#include <wx/imaglist.h>
#include <wx/thread.h>
uint16_t refreshDeviceState();

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

class LibreScribe__Frame;

class BackgroundMonitor : public wxThread
    {
    public:
        BackgroundMonitor(LibreScribe__Frame* handler) : wxThread(wxTHREAD_DETACHED) {
            m_pHandler = handler;
        };
        ~BackgroundMonitor();
    protected:
        virtual ExitCode Entry();
        LibreScribe__Frame* m_pHandler;
    };

class LibreScribe__Frame: public GUIFrame
{
    friend class LibreScribe__App;
    public:
        LibreScribe__Frame(wxFrame *frame);
        ~LibreScribe__Frame();
        BackgroundMonitor *m_pThread;
        wxCriticalSection m_pThreadCS;    // protects the m_pThread pointer
        void addAudioClipToList(audioClipInfo info);
        void addApplicationToList(applicationInfo info);
    private:
        void StartBackgroundMonitor();
        virtual void OnClose(wxCloseEvent& event);
        void setupPageHierarchy();
        void setupLists();
};

#endif // LibreScribe__MAIN_H
