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

#include "DeviceInformation.h"

//This function will strip out all non-numeric characters from a char*
int stripNonNumericChars(char* s) {
    int res;
    bool success = sscanf(s, "%d%%", &res) == 1;
    return res;
}

DeviceInformation::~DeviceInformation() {
    smartpen_disconnect(device_handle);
}

static void showStorageInformation(xmlNode * a_node) {
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

static void showBatteryStatistics(xmlNode * a_node) {
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

//searches through a node until an element with the specified node is detected.
xmlNode * getSubNode(xmlNode *root, const xmlChar *node) {
    for(xmlNode *cur_node = root->children; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE  && !xmlStrcmp(cur_node->name, node)) {
            return cur_node;
        }
    }
}

float DeviceInformation::getBatteryVoltage(xmlNode *root) {
    //first we need to locate the "peninfo" node which contains the "battery" node
    xmlNode *cur_node = getSubNode(root, (const xmlChar *)"peninfo");
    //once we find our "peninfo" node, search through the nodes, looking for the "battery" node
    cur_node = getSubNode(cur_node, (const xmlChar *)"battery");
    char* voltage = (char*)xmlGetProp(cur_node, (const xmlChar*)"voltage");
    return stripNonNumericChars(voltage);
}

int DeviceInformation::getBatteryRemaining(xmlNode *root) {
    //first we need to locate the "peninfo" node which contains the "battery" node
    xmlNode *cur_node = getSubNode(root, (const xmlChar *)"peninfo");
    //once we find our "peninfo" node, search through the nodes, looking for the "battery" node
    cur_node = getSubNode(cur_node, (const xmlChar *)"battery");
    char* level = (char*)xmlGetProp(cur_node, (const xmlChar*)"level");
    return stripNonNumericChars(level);
}

long long int DeviceInformation::getFreeBytes(xmlNode *root) {
    //first we need to locate the "peninfo" node which contains the "memory" node
    xmlNode *cur_node = getSubNode(root, (const xmlChar *)"peninfo");
    //once we find our "peninfo" node, search through the nodes, looking for the "memory" node
    cur_node = getSubNode(cur_node, (const xmlChar *)"memory");
    char* bytes = (char*)xmlGetProp(cur_node, (const xmlChar*)"freebytes");
    return stripNonNumericChars(bytes);
}

long long int DeviceInformation::getTotalBytes(xmlNode *root) {
    //first we need to locate the "peninfo" node which contains the "memory" node
    xmlNode *cur_node = getSubNode(root, (const xmlChar *)"peninfo");
    //once we find our "peninfo" node, search through the nodes, looking for the "memory" node
    cur_node = getSubNode(cur_node, (const xmlChar *)"memory");
    char* bytes = (char*)xmlGetProp(cur_node, (const xmlChar*)"totalbytes");
    return stripNonNumericChars(bytes);
}
