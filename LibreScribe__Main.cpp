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

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "LibreScribe__Main.h"
#include "DeviceInformation.h"
#include "Smartpen.h"

struct usb_device *dev;
wxImageList* treeImages;

LibreScribe__Frame::LibreScribe__Frame(wxFrame *frame) : GUIFrame(frame) {
    printf("LibreScribe Alpha version 0.02, written by Dylan Taylor\n");
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(_("The status bar is still a work-in-progress."), 0);
#endif
    //the following code makes it so the page tree is automatically fitted to the window.
    wxBoxSizer* pageTreeSizer = new wxBoxSizer( wxVERTICAL );
    pageTreeSizer->Add(pageTree, true, wxEXPAND | wxALL, 5);
    pagesTab->SetSizer(pageTreeSizer);
    setupPageHierarchy();
    setupLists();
    doRefreshDeviceState();
}

LibreScribe__Frame::~LibreScribe__Frame() { //destructor
}

void LibreScribe__Frame::setupPageHierarchy() {
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
    pageTree->SetWindowStyle(wxTR_NO_BUTTONS | wxTR_NO_LINES);
    //this is a huge work in progress... tons of work to do!
}

const wxString audioColumns[] = {_("Session Name"),
                                 _("Duration"),
                                 _("Recorded"),
                                 _("File Size")};

const wxString appColumns[] = {_("Application Name"),
                                 _("Version"),
                                 _("File Size")};

void LibreScribe__Frame::addAudioClipToList(audioClipInfo info) {
    audioList->InsertItem(0, info.name);
    audioList->SetItem(0, 1, info.duration);
    audioList->SetItem(0, 2, info.date);
    audioList->SetItem(0, 3, info.size);
}

void LibreScribe__Frame::addApplicationToList(applicationInfo info) {
    appList->InsertItem(0, info.name);
    appList->SetItem(0, 1, info.version);
    appList->SetItem(0, 2, info.size);
}

void LibreScribe__Frame::setupLists() {
    const int audio_column_width = 180;
    const int app_column_width = 240;
    //first, set up the list control sizers
    wxBoxSizer* audioSizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer* appSizer = new wxBoxSizer( wxVERTICAL );
    audioSizer->Add(audioList, true, wxEXPAND | wxALL, 5);
    appSizer->Add(appList, true, wxEXPAND | wxALL, 5);
    audioTab->SetSizer(audioSizer);
    appTab->SetSizer(appSizer);
    audioClipInfo sampleClipInfo = {_("Sample Audio Clip Info"), _("13:37"), _("11/11/2011 11:11AM"), _("421.8 KiB")};
    for (int i = 0; i < (sizeof(audioColumns)/sizeof(wxString)); i++) {
        audioList->InsertColumn(i, audioColumns[i], wxLIST_FORMAT_LEFT, audio_column_width);
    }
    addAudioClipToList(sampleClipInfo);
    applicationInfo sampleAppInfo = {_("Sample LiveScribe Application"), _("1.0"), _("1.44 MiB")};
    for (int i = 0; i < (sizeof(appColumns)/sizeof(wxString)); i++) {
        appList->InsertColumn(i, appColumns[i], wxLIST_FORMAT_LEFT, app_column_width);
    }
    addApplicationToList(sampleAppInfo);
}

void LibreScribe__Frame::OnClose(wxCloseEvent &event) {
    Destroy();
}

void LibreScribe__Frame::OnQuit(wxCommandEvent &event) {
    Destroy();
}

void LibreScribe__Frame::OnInfo(wxCommandEvent &event) {
    if (dev != NULL) {
        obex_t *handle = smartpen_connect(dev->descriptor.idVendor, dev->descriptor.idProduct);
        if (handle != NULL) {
            wxString deviceName("My Smartpen", wxConvUTF8);
            DeviceInformation d(this, deviceName,dev->descriptor.idProduct,handle);
            d.ShowModal(); //display the information dialog
        } else {
            wxMessageBox(_("A connection to your Smartpen could not be established. Is it already in use?"), _("Smartpen Connection Failure"));
        }
    } else {
        doRefreshDeviceState();
    }
}

void LibreScribe__Frame::doRefreshDeviceState() {
    printf("Searching for your Smartpen... ");
    statusBar->SetStatusText(_("Searching for a compatible smartpen device..."), 1);
    //uint16_t result = refreshDeviceState();
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
        }
    } catch(...) {
        printf("Failed to search for your Smartpen\n");
    }
    return;
}

uint16_t LibreScribe__Frame::refreshDeviceState() {
    printf("Searching for your Smartpen... ");
    dev = findSmartpen();
    if (dev == NULL) { //If the smartpen wasn't found the function will have returned NULL
        printf("Sorry! No compatible smartpen device found!\n");
        return 0x0000;
    } else {
        if (dev->descriptor.idProduct == LS_PULSE) {
            printf("LiveScribe Pulse(TM) Smartpen Detected!\n");
        } else if (dev->descriptor.idProduct == LS_ECHO) {
            printf("LiveScribe Echo(TM) Smartpen Detected!\n");
        } else {
            printf("Unknown LiveScribe device detected! Attempting to use this device anyways...\n");
        }
        return dev->descriptor.idProduct;
    }
}

void LibreScribe__Frame::OnRefresh(wxCommandEvent &event) {
    doRefreshDeviceState();
//    wxMessageBox(_("Refreshing device information..."), _("LibreScribe Smartpen Manager"));
}

void LibreScribe__Frame::OnAbout(wxCommandEvent &event) {
    wxMessageBox(_("Written by Dylan Taylor. A large portion of the code is taken from libsmartpen, written by Steven Walter. This is alpha quality software. Use in production environments is NOT recommended."), _("LibreScribe Smartpen Manager"));
}
