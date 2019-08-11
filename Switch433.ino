/*
* Scan for RF signals and map them for later usage
*/
#include <NewRemoteReceiver.h>
//#include <RemoteReceiver.h>
#include "Configuration.h"

int mainLoopCounter = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);// Initialize the LED_BUILTIN pin as an output
  Serial.begin(115200);//for debugging purposes.
  config_init();
  wifi_init();
  rf_init();
}

void loop() {
  wifi_handle_client();
}
