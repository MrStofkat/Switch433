/*
* Scan for RF signals and map them for later usage
*/

#include <NewRemoteReceiver.h>
#include <RemoteReceiver.h>
#include <EEPROM.h>



void setup() {
  Serial.begin(115200);//for debugging purposes.
  rf_module_start();
  wifi_module_start();
}

void loop() {
  wifi_module_handle_request();
  
}

      
