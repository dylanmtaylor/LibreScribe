#include "DeviceInformation.h"

//DeviceInformation::DeviceInformation(wxWindow* parent) : DeviceInfo(parent)
//{
//    //ctor
//}

DeviceInformation::~DeviceInformation()
{
    //dtor
}

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


int DeviceInformation::getBatteryRemaining(xmlNode *a_node) {
    xmlNode *cur_node = NULL;
    //scan for battery element
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (strcmp((const char*)cur_node->name,"battery") == 0) {
                //printf("node type: Element, name: %s\n", cur_node->name);
                char* level = (char*)xmlGetProp(cur_node, (const xmlChar*)"level");
                return atoi(level);
            }
        }
        getBatteryRemaining(cur_node->children);
    }
}
