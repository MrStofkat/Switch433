/*
* Scan for RF signals and map them for later usage
*/
#include <NewRemoteReceiver.h>
#include <RemoteReceiver.h>

void setup() {
  Serial.begin(115200);//for debugging purposes.
  config_init();
  wifi_init();
  rf_module_start();
}

void loop() {
  wifi_handle_client();
}

      
