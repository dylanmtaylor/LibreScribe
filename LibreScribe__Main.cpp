/***************************************************************
 * Name:      LibreScribe__Main.cpp
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

#include "LibreScribe__Main.h"
#include "DeviceInformation.h"
#include "Smartpen.h"

struct usb_device *dev;
struct udev *udev;
struct udev_device *udevice;
struct udev_list_entry *devices, *dev_list_entry;
struct udev_enumerate *enumerate;
struct udev_monitor *mon;
int fd;

//This method constantly runs as a thread in the background, checking to see if devices are added/removed
//This method is based off of this code: http://www.signal11.us/oss/udev/udev_example.c
static void backgroundMonitor() {
	    fd_set fds;
		struct timeval tv;
		int ret;
    while(true) {
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		ret = select(fd+1, &fds, NULL, NULL, &tv);
		// Check if our file descriptor has received data.
		if (ret > 0 && FD_ISSET(fd, &fds)) {
            udevice = udev_monitor_receive_device(mon);
		    printf("We should be checking for device changes here...\n");
		    udev_device_unref(udevice);
//          refreshDeviceState();
		}
        usleep(250*1000);
	}
}

LibreScribe__Frame::LibreScribe__Frame(wxFrame *frame)
    : GUIFrame(frame)
{
    printf("LibreScribe Alpha version 0.01, written by Dylan Taylor\n");
#if wxUSE_STATUSBAR
    statusBar->SetStatusText(_("The status bar is still a work-in-progress."), 0);
#endif
    udev = udev_new();
    mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_enable_receiving(mon);
    fd = udev_monitor_get_fd(mon);
    std::thread t(backgroundMonitor);
    t.detach();
    refreshDeviceState();
}

void LibreScribe__Frame::updateStatusbar() {

}

LibreScribe__Frame::~LibreScribe__Frame()
{
}

void LibreScribe__Frame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void LibreScribe__Frame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void LibreScribe__Frame::OnInfo(wxCommandEvent &event) {
    obex_t *handle = smartpen_connect(dev->descriptor.idVendor, dev->descriptor.idProduct);
    wxString deviceName("My Smartpen", wxConvUTF8);
    DeviceInformation d(this, deviceName,dev->descriptor.idProduct,handle);
    d.ShowModal(); //display the information dialog
}

void LibreScribe__Frame::refreshDeviceState() {
    statusBar->SetStatusText(_("Searching for a compatible smartpen device..."), 1);
    printf("Searching for your Smartpen... ");
    dev = findSmartpen();
    if (dev == NULL) { //If the smartpen wasn't found the function will have returned NULL
        this->mainToolbar->EnableTool(idToolbarInfo,false);
        printf("Sorry! No compatible smartpen device found!\n");
//        statusBar->SetStatusText(_("Please connect your smartpen device."), 1);
        statusBar->SetStatusText(_("Unable to locate a compatible Smartpen device"), 1);
    } else {
        if (dev->descriptor.idProduct == LS_PULSE) {
            this->mainToolbar->EnableTool(idToolbarInfo,true);
            printf("LiveScribe Pulse(TM) Smartpen Detected!\n");
            statusBar->SetStatusText(_("LiveScribe Pulse(TM) Smartpen Detected!"), 1);
        } else if (dev->descriptor.idProduct == LS_ECHO) {
            this->mainToolbar->EnableTool(idToolbarInfo,true);
            printf("LiveScribe Echo(TM) Smartpen Detected!\n");
            statusBar->SetStatusText(_("LiveScribe Echo(TM) Smartpen Detected!"), 1);
        } else {
            printf("Unknown LiveScribe device detected! Attempting to use this device anyways...\n");
            statusBar->SetStatusText(_("Unknown LiveScribe Device Detected!"), 1);
        }
    }
}

void LibreScribe__Frame::OnRefresh(wxCommandEvent &event) {
    refreshDeviceState();
//    wxMessageBox(_("Refreshing device information..."), _("LibreScribe Smartpen Manager"));
}

void LibreScribe__Frame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox(_("Written by Dylan Taylor. A large portion of the code is taken from libsmartpen, written by Steven Walter. This is alpha quality software. Use in production environments is NOT recommended."), _("LibreScribe Smartpen Manager"));
}
