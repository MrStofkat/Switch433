#include <NewRemoteReceiver.h>
#include <RemoteReceiver.h>
#include <EEPROM.h>


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
    RemoteReceiver::init(1, 3, gotUniversalCode);
    pinMode(LED_BUILTIN, OUTPUT);
    
    storedAddress = EEPROMReadLong(0);
    storedUnit = EEPROM.read(4);
    Serial.println();
    if(storedAddress>0){
      Serial.println("Got stored address");
      Serial.println(storedAddress);
      hasPendingDevice=false;
    }else {
      Serial.println("Aint got stored address");
    }
} 

void rf_sendSignal(int deviceID, boolean signal) {
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
      EEPROMWriteLong(0,storedAddress); 
      EEPROM.write(4,storedUnit);
      hasPendingDevice=false;
      //addDevice(pendingDeviceName, address, unit);
      return;
  }
  
  if(storedAddress == receivedCode.address && storedUnit == receivedCode.unit){
      switch (receivedCode.switchType) {
          case NewRemoteCode::off:
            digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
            Serial.println(" off");
            break;
          case NewRemoteCode::on:
            digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
            Serial.println(" on");
            break;
          case NewRemoteCode::dim:
            Serial.print(" dim");
            break;
      }

      if (receivedCode.dimLevelPresent) {
        Serial.print(", dim level: ");
        Serial.print(receivedCode.dimLevel);
      }

  }

}

long EEPROMReadLong(long address)
      {
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}


//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.
void EEPROMWriteLong(int address, long value) {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
}


