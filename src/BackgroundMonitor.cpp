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

#include "../GUIFrame.h"

struct udev *udev;
struct udev_device *udevice;
struct udev_list_entry *devices, *dev_list_entry;
struct udev_enumerate *enumerate;
struct udev_monitor *mon;
int fd;

wxThread::ExitCode BackgroundMonitor::Entry() {
    fd_set fds;
    struct timeval tv;
    int ret;
    printf("Starting background monitor...\n");
    udev = udev_new();
    mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_enable_receiving(mon);
    fd = udev_monitor_get_fd(mon);

    while (!TestDestroy()) {
        //wxMessageBox(_("Hello from the background monitor thread!"));
        //This constantly runs as a thread in the background, checking to see if devices are added/removed
        //This is based off of this code: http://www.signal11.us/oss/udev/udev_example.c
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
                wxMutexGuiEnter();
                m_pHandler->doRefreshDeviceState();
                wxMutexGuiLeave();
            } catch(...) {
                printf("Error refreshing device state");
            }
            printf("Done refreshing device state.\n");
        }
        usleep(250*1000);
    }
    printf("Exited background monitor while loop\n");
    // signal the event handler that this thread is going to be destroyed
    // NOTE: here we assume that using the m_pHandler pointer is safe,
    //       (in this case this is assured by the MyFrame destructor)

    return (wxThread::ExitCode)0;     // success
}

BackgroundMonitor::~BackgroundMonitor()
{
    wxCriticalSectionLocker enter(m_pHandler->m_pThreadCS);
    // the thread is being destroyed; make sure not to leave dangling pointers around
    m_pHandler->m_pThread = NULL;
}
