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

static void
showStorageInformation(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;
    //scan for battery element
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (strcmp((const char*)cur_node->name,"memory") == 0) {
                //printf("node type: Element, name: %s\n", cur_node->name);
                char* free = (char*)xmlGetProp(cur_node, (const xmlChar*)"freebytes");
                char* total = (char*)xmlGetProp(cur_node, (const xmlChar*)"totalbytes");
                printf("Storage: %.02f of %.02f MB remaining\n", ((float)atoi(free)/(float)1048576), ((float)atoi(total)/(float)1048576));
                return;
            }
        }
        showStorageInformation(cur_node->children);
    }
}

static void
showBatteryStatistics(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;
    //scan for battery element
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (strcmp((const char*)cur_node->name,"battery") == 0) {
                //printf("node type: Element, name: %s\n", cur_node->name);
                char* voltage = (char*)xmlGetProp(cur_node, (const xmlChar*)"voltage");
                char* level = (char*)xmlGetProp(cur_node, (const xmlChar*)"level");
                printf("Battery Level: %s, Voltage: %s\n", level, voltage);
                return;
            }
        }
        showBatteryStatistics(cur_node->children);
    }
}

static void
showSmartpenTimeInfo(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;
    //scan for battery element
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (strcmp((const char*)cur_node->name,"time") == 0) {
                //printf("node type: Element, name: %s\n", cur_node->name);
                long devtime = (long)strtol((const char*)xmlGetProp(cur_node, (const xmlChar*)"absolute"),NULL,0);
                printf("Absolute Device Time: %ld\n", devtime);
                return;
            }
        }
        showSmartpenTimeInfo(cur_node->children);
    }
}

bool LibreScribe__App::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler());
    LibreScribe__Frame* frame = new LibreScribe__Frame(0L);
    frame->Show();
//    getPenInfo();

    return true;
}
