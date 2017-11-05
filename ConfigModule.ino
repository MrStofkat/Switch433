#include <ArduinoJson.h>
#include "FS.h"


//Describes a device
struct Device {
   String deviceName;
   long address;
   int unit;
   int period;
};

//Describes the actual configuration structure
struct Configuration {
  const char * ssid;
  const char * password;
  int deviceCount;
  Device devices[20];
} configuration;

void config_init() {

  Serial.println("Mounting FS...");
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  } else {
    _config_load();  
  } 
}


void config_storeWifiCredentials(const char * ssid, const char * password) {
  configuration.ssid = ssid;
  configuration.password = password;
  _config_save();
}

void config_addDevice(const char * deviceName, long address, int unit, int period) {

}

bool _config_load() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  configuration.ssid = json["ssid"];
  configuration.password = json["password"];
  Serial.print("Loaded ssid: ");
  Serial.println(configuration.ssid);
  Serial.print("Loaded password: ");
  Serial.println(configuration.password);
  return true;
}

bool _config_save() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["ssid"] = configuration.ssid;
  json["password"] = configuration.password;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  json.printTo(configFile);
  return true;
}

