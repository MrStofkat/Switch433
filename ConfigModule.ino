#include <ArduinoJson.h>
#include "FS.h"

Configuration configuration;

void config_init() {

  Serial.println("Mounting FS...");
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  } else {
    _config_load();
  }
}

bool _config_load() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<800> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }
  configuration.ssid = "" + json["ssid"].as<String>();
  configuration.password = "" + json["password"].as<String>();
  // Loop through the devices
  JsonArray& jsonDevices = json["devices"].as<JsonArray>();
  for (int i = 0; i < jsonDevices.size(); i++) {
    JsonObject& device = jsonDevices.get<JsonObject>(i);
    configuration.devices[configuration.deviceCount++] = (Device) {
     device["address"].as<unsigned long>(),
     device["unit"].as<unsigned long>(),
     device["period"].as<unsigned long>(),
     false,
     device["name"].as<String>()
    };
  }

  Serial.print("Loaded ssid: ");
  Serial.println(configuration.ssid);

  return true;
}


void config_storeWifiCredentials(String ssid, String password) {
  configuration.ssid = ssid;
  configuration.password = password;
  _config_save();
}

void config_addDevice(String deviceName,  unsigned long address,  unsigned long unit,  unsigned long period) {
  Serial.println("Saving to config:" + deviceName);
  Serial.print("Code: ");
  Serial.print(address);
  Serial.print(", unit: ");
  Serial.print(unit);
  Serial.print(", period duration: ");
  Serial.print(period);
  Serial.println("us.");
  configuration.devices[configuration.deviceCount++] =
  (Device) {
    address,
    unit,
    period,
    false,
    deviceName,
  };
  _config_save();
}

bool _config_save() {
  // Create a dynamic json buffer and save all the data
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["ssid"] = configuration.ssid;
  json["password"] = configuration.password;
  // Check if we already have any devices and save them if needed
  if (configuration.deviceCount > 0) {
    JsonArray& devices = json.createNestedArray("devices");
    for (int i = 0; i < configuration.deviceCount; i++) {
      Serial.print("Saving code: ");
      Serial.print(configuration.devices[i].address);
      Serial.print(", unit: ");
      Serial.print(configuration.devices[i].unit);
      Serial.print(", period duration: ");
      Serial.print(configuration.devices[i].period);
      Serial.println("us.");
      String serializedAddress = String(configuration.devices[i].address);
      JsonObject& device = jsonBuffer.createObject();
      device["name"] = configuration.devices[i].deviceName;
      device["address"] = configuration.devices[i].address;
      device["unit"] = configuration.devices[i].unit;
      device["period"] = configuration.devices[i].period;
      devices.add(device);
    }
  }

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  json.printTo(configFile);
  return true;
}
