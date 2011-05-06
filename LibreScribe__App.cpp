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

#include "LibreScribe__App.h"
#include "LibreScribe__Main.h"
IMPLEMENT_APP(LibreScribe__App);

bool LibreScribe__App::OnInit() {
    wxImage::AddHandler(new wxPNGHandler());
//    udev = udev_new();
//    mon = udev_monitor_new_from_netlink(udev, "udev");
//    udev_monitor_enable_receiving(mon);
//    fd = udev_monitor_get_fd(mon);
    LibreScribe__Frame* frame = new LibreScribe__Frame(0L);
    frame->Show();
//    std::thread t(backgroundMonitor, frame);
//    t.detach();
    return true;
}
