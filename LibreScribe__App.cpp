/***************************************************************
 * Name:      LibreScribe__App.cpp
 * Purpose:   Code for Application Class
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

#include "LibreScribe__App.h"
#include "LibreScribe__Main.h"
IMPLEMENT_APP(LibreScribe__App);

struct udev *udev;
struct udev_device *udevice;
struct udev_list_entry *devices, *dev_list_entry;
struct udev_enumerate *enumerate;
struct udev_monitor *mon;
int fd;

//This method constantly runs as a thread in the background, checking to see if devices are added/removed
//This method is based off of this code: http://www.signal11.us/oss/udev/udev_example.c
void backgroundMonitor(LibreScribe__Frame* frame) {
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
            printf("Refreshing device state...\n");
            try {
                //frame->doRefreshDeviceState(); //commented out because it causes crashes
                frame->refreshDeviceState();
            } catch(...) {
                printf("Error refreshing device state\n");
            }
            printf("Done refreshing device state.\n");
		}
        usleep(250*1000);
	}
}

bool LibreScribe__App::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler());
    udev = udev_new();
    mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_enable_receiving(mon);
    fd = udev_monitor_get_fd(mon);
    LibreScribe__Frame* frame = new LibreScribe__Frame(0L);
    frame->Show();
    std::thread t(backgroundMonitor, frame);
    t.detach();
    return true;
}
