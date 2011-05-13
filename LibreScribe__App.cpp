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
//(*AppHeaders
#include "GUIFrame.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(LibreScribe__App);

bool LibreScribe__App::OnInit() {
	//(*AppInitialize
	bool wxsOK = true;
	wxInitAllImageHandlers();
	if ( wxsOK )
	{
		LibreScribe__Frame* Frame = new LibreScribe__Frame(0L);
		Frame->Show();
		SetTopWindow(Frame);
	}
	//*)
	return wxsOK;
}
