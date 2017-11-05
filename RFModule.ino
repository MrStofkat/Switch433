#include <NewRemoteReceiver.h>
#include <RemoteReceiver.h>


bool hasPendingDevice=true;
const char *pendingDeviceName;
long storedAddress =0;
int storedUnit=0;
bool rf_module_is_running =false;

void rf_module_start(){
  
    // Initialize receiver on interrupt 0 (= digital pin 2), calls the callback "showCode"
    // after 2 identical codes have been received in a row. (thus, keep the button pressed
    // for a moment)
    //
    // See the interrupt-parameter of attachInterrupt for possible values (and pins)
    // to connect the receiver.
    NewRemoteReceiver::init(4, 3, gotKlikAanKlikUitCode);
    //RemoteReceiver::init(1, 3, gotUniversalCode);
    pinMode(LED_BUILTIN, OUTPUT);
   
} 

void rf_sendSignal(int deviceID, boolean signal) {
    //NewRemoteTransmitter transmitter(receivedCode.address, 11, receivedCode.period);
   //Device device = controller.devices[deviceID];
   //TODO
}

// Callback function is called only when a valid code is received.
void gotUniversalCode(unsigned long receivedCode, unsigned int period) {
  // Note: interrupts are disabled. You can re-enable them if needed.
  
  // Print the received code.
  Serial.print("Code: ");
  Serial.print(receivedCode);
  Serial.print(", period duration: ");
  Serial.print(period);
  Serial.println("us.");

}

// Callback function is called only when a valid code is received.
void gotKlikAanKlikUitCode(NewRemoteCode receivedCode) {
  
  // Note: interrupts are disabled. You can re-enable them if needed.
  if(hasPendingDevice){
      Serial.println("Stored new address");
      storedAddress = receivedCode.address;
      storedUnit = receivedCode.unit;
      hasPendingDevice=false;
      config_addDevice(pendingDeviceName, receivedCode.address, receivedCode.unit, receivedCode.period);
      return;
  }
}



