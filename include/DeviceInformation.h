#ifndef DEVICEINFORMATION_H
#define DEVICEINFORMATION_H
#include "../GUIFrame.h"
#include "usb_device.h"

class DeviceInformation : public DeviceInfo
{
    public:
        DeviceInformation(wxWindow* parent) : DeviceInfo(parent){
            battery = 0;
        };
        virtual ~DeviceInformation();
    protected:
    private:
        int battery;
};

//class LibreScribe__Frame: public GUIFrame
//{
//    public:
//        LibreScribe__Frame(wxFrame *frame);
//        ~LibreScribe__Frame();
//        void updateStatusbar();
//    private:
//        void refreshDeviceState();
//        virtual void OnClose(wxCloseEvent& event);
//        virtual void OnQuit(wxCommandEvent& event);
//        virtual void OnAbout(wxCommandEvent& event);
//        virtual void OnRefresh(wxCommandEvent& event);
//        virtual void OnInfo(wxCommandEvent& event);
//};

#endif // DEVICEINFORMATION_H
