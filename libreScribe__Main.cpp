/***************************************************************
 * Name:      libreScribe__Main.cpp
 * Purpose:   Code for Application Frame
 * Author:    Dylan Taylor (aliendude5300@gmail.com)
 * Created:   2011-04-07
 * Copyright: Dylan Taylor (http://dylanmtaylor.com/)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "libreScribe__Main.h"

struct usb_device *dev;

static struct usb_device *findSmartpen()
{
  struct usb_bus *bus;
  struct usb_device *dev;
  struct usb_bus *busses;

  usb_init();
  usb_find_busses();
  usb_find_devices();
  busses = usb_get_busses();

  for (bus = busses; bus; bus = bus->next) {
    for (dev = bus->devices; dev; dev = dev->next) {
      if ((dev->descriptor.idVendor == LS_VENDOR_ID)) {
        return dev;
      }
    }
  }
  return NULL;
}

libreScribe__Frame::libreScribe__Frame(wxFrame *frame)
    : GUIFrame(frame)
{
    printf("libreScribe Alpha version 0.01, written by Dylan Taylor\n");
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(_("The status bar is still a work-in-progress."), 0);
#endif
    refreshDeviceState();
}

void libreScribe__Frame::updateStatusbar() {

}

libreScribe__Frame::~libreScribe__Frame()
{
}

void libreScribe__Frame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void libreScribe__Frame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void libreScribe__Frame::OnInfo(wxCommandEvent &event) {

}

void libreScribe__Frame::refreshDeviceState() {
    statusBar->SetStatusText(_("Searching for a compatible smartpen device..."), 1);
    printf("Searching for your Smartpen... ");
    dev = findSmartpen();
    //If the smartpen wasn't found the function will have returned NULL
    if (dev == NULL) {
        printf("Sorry! No compatible smartpen device found!\n");
//        statusBar->SetStatusText(_("Please connect your smartpen device."), 1);
        statusBar->SetStatusText(_("Unable to locate a compatible Smartpen device"), 1);
    } else {
        if (dev->descriptor.idProduct == LS_PULSE) {
            printf("LiveScribe Pulse(TM) Smartpen Detected!\n");
            statusBar->SetStatusText(_("LiveScribe Pulse(TM) Smartpen Detected!"), 1);
            //exit(2);
        } else {
            printf("Unknown LiveScribe device detected! Attempting to use this device anyways...\n");
            statusBar->SetStatusText(_("Unknown LiveScribe Device Detected!"), 1);
        }
    }

}

void libreScribe__Frame::OnRefresh(wxCommandEvent &event) {
    refreshDeviceState();
//    wxMessageBox(_("Refreshing device information..."), _("LibreScribe Smartpen Manager"));
}

void libreScribe__Frame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(_("Written by Dylan Taylor. A large portion of the code is taken from libsmartpen, written by Steven Walter. This is alpha quality software. Use in production environments is NOT recommended."), _("LibreScribe Smartpen Manager"));
}
