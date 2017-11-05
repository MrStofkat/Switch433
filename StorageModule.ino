#include <EEPROM.h>
#include <ArduinoJson.h>
struct Device {
   String deviceName;
   long address;
   int unit;
 };
struct Controller {
   String ssid;
   String password;
   int deviceCount;
   Device devices[20];
 } controller;


void addDevice(String deviceName, long address, int unit) {
  Device device = {deviceName, address, unit};
  controller.devices[controller.deviceCount++] = device;
}
