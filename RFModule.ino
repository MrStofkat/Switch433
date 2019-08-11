#include <NewRemoteReceiver.h>
#include <NewRemoteTransmitter.h>
//#include <RemoteReceiver.h> // For older RF devices you might need this one

#define RF_OUT_PIN 13
#define RF_IN_PIN 4

bool hasPendingDevice = false;
String pendingDeviceName;
long storedAddress = 0;
int storedUnit = 0;

void rf_sendSignal(int id, boolean state) {
  Device device = configuration.devices[id];
  NewRemoteReceiver::disable();
  NewRemoteTransmitter transmitter(device.address, RF_OUT_PIN, device.period);
  transmitter.sendUnit(device.unit, state);
  interrupts();
  delay(100);
  NewRemoteReceiver::enable();
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
void gotKlikAanKlikUitCode(unsigned int period, unsigned long address, unsigned long groupBit, unsigned long unit, unsigned long switchType) {
  Serial.print("Code: ");
  Serial.print(address);
  Serial.print(", unit: ");
  Serial.print(unit);
  Serial.print(", period duration: ");
  Serial.print(period);
  Serial.println("us.");
  // Note: interrupts are disabled. You can re-enable them if needed.
  if (hasPendingDevice) {
    Serial.println("Stored new address");
    Serial.println(address);
    hasPendingDevice = false;
    config_addDevice(pendingDeviceName, address, unit, period);
    return;
  }
}

void rf_init() {
  // Initialize receiver on interrupt 0 (= digital pin 2), calls the callback "showCode"
  // after 2 identical codes have been received in a row. (thus, keep the button pressed
  // for a moment)
  // See the interrupt-parameter of attachInterrupt for possible values (and pins)
  // to connect the receiver.
  NewRemoteReceiver::init((int8_t)RF_IN_PIN, (byte)2, gotKlikAanKlikUitCode);
  //RemoteReceiver::init(1, 3, gotUniversalCode);
  pinMode(LED_BUILTIN, OUTPUT);
}
