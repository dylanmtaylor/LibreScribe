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

bool LibreScribe__App::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler());
    LibreScribe__Frame* frame = new LibreScribe__Frame(0L);
    frame->Show();
    return true;
}
