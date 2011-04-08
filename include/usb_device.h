#ifndef USB_DEVICE_H_INCLUDED
#define USB_DEVICE_H_INCLUDED
#include <usb.h>

#define LS_VENDOR_ID 0x1cfb //LiveScribe Vendor ID
#define LS_PULSE 0x1020 //LiveScribe Pulse(TM) SmartPen
#define LS_ECHO 0x0000 //currently unknown

static struct usb_device *findSmartpen()
{
  struct usb_bus *bus;
  struct usb_device *dev;
  struct usb_bus *busses;

  usb_init();
  usb_find_busses();
  usb_find_devices();
  busses = usb_get_busses();

  for (bus = busses; bus; bus = bus->next) {
    for (dev = bus->devices; dev; dev = dev->next) {
      if ((dev->descriptor.idVendor == LS_VENDOR_ID)) {
        return dev;
      }
    }
  }
  return NULL;
}

#endif // USB_DEVICE_H_INCLUDED
