/***************************************************************
 * Name:      libreScribe__Main.h
 * Purpose:   Defines Application Frame
 * Author:    Dylan Taylor (aliendude5300@gmail.com)
 * Created:   2011-04-07
 * Copyright: Dylan Taylor (http://dylanmtaylor.com/)
 * License:
 **************************************************************/

#ifndef LIBRESCRIBE__MAIN_H
#define LIBRESCRIBE__MAIN_H
#include <openobex/obex.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <arpa/inet.h>
#include <usb.h>
#include <glib-2.0/glib.h>
#include "LibreScribe__App.h"
#define LS_VENDOR_ID 0x1cfb //LiveScribe Vendor ID
#define LS_PULSE 0x1020 //LiveScribe Pulse(TM) SmartPen
#define LS_ECHO 0x0000 //currently unknown
#include "GUIFrame.h"

class libreScribe__Frame: public GUIFrame
{
    public:
        libreScribe__Frame(wxFrame *frame);
        ~libreScribe__Frame();
        void updateStatusbar();
    private:
        void refreshDeviceState();
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
        virtual void OnRefresh(wxCommandEvent& event);
        virtual void OnInfo(wxCommandEvent& event);
};

#endif // LIBRESCRIBE__MAIN_H
