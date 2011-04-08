/***************************************************************
 * Name:      LibreScribe__Main.h
 * Purpose:   Defines Application Frame
 * Author:    Dylan Taylor (aliendude5300@gmail.com)
 * Created:   2011-04-07
 * Copyright: Dylan Taylor (http://dylanmtaylor.com/)
 * License:
 **************************************************************/

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
#include <thread>
uint16_t refreshDeviceState();
class LibreScribe__Frame: public GUIFrame
{
    public:
        LibreScribe__Frame(wxFrame *frame);
        ~LibreScribe__Frame();
        void doRefreshDeviceState();
    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
        virtual void OnRefresh(wxCommandEvent& event);
        virtual void OnInfo(wxCommandEvent& event);
};

#endif // LibreScribe__MAIN_H
