/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ap_ssid = "Stofkat Home Automation";
const char *ap_password = "thereisnospoon";

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
  server.send(200, "text/html", "<marquee>You are connected</marquee><br/><marquee>to the most awesome</marquee><br/><marquee>home automation system in existance!</marquee>");
}

void handleRFScanMode(){
    hasPendingDevice=true;
    server.send(200, "text/html", "<h1>Scanning for new RF signal..</h1>");
}

void handleWifiConnect() {
    Serial.println("handleWifiConnect ");
    Serial.println(server.arg("plain"));
    DynamicJsonBuffer  jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));
    if (!root.success()) {
      Serial.println("parsing JSON failed!");
    }
    server.send(200, "application/json", "{\"message\": \"Now connecting to the WiFi network\"}");
    String ssid = root["ssid"].as<String>();
    String password = root["password"].as<String>();
    Serial.println("handleWifiConnect END");
    WiFi.begin(ssid.c_str(),password.c_str());
    config_storeWifiCredentials(ssid, password);
    Serial.println(configuration.ssid);
}

void handleReadTest() {
    Serial.println("handleSetDevice ");
    Serial.println(server.arg("plain"));
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));
    int deviceID = root["deviceID"];
    boolean state = root["state"];
    Serial.print("ssid: ");
    Serial.println(configuration.ssid);
    Serial.print("password string: ");
    Serial.println(configuration.password);
 }

void handleSetDevice() {
    Serial.println("handleSetDevice ");
    Serial.println(server.arg("plain"));
    StaticJsonBuffer<200> jsonBuffer;
    server.send(200, "application/json", "{\"message\": \"Sending signal\"}");
    JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));
    int deviceID = root["deviceID"];
    boolean state = root["state"];
    rf_sendSignal(deviceID, state);
}

 
void handleRemoveDevice() {
    Serial.println("handleRemoveDevice ");
    Serial.println(server.arg("plain"));
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(server.arg("json"));
    server.send(200, "application/json", "{\"message\": \"Removing device\"}");
    int deviceID = root["deviceID"];
}

void handleAddDevice() {
    Serial.println("handleAddDevice ");
    Serial.println(server.arg("plain"));
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(server.arg("json"));
    server.send(200, "application/json", "{\"message\": \"Scanning for new RF433 signal\"}");
    hasPendingDevice = true;
    pendingDeviceName = root["name"].as<String>();
    //addDevice("",2,2);
}

void handleListDevices() {
  //It is normally bad practice to compile your own JSON
  //but in this case it saved a lot of memory on the uC
  //plus it significantly speeds up the process
  String devicesJSON ="[";
  for(int i=0; i<configuration.deviceCount; i++){
    devicesJSON += "{"
    devicesJSON += "\"id\": \"" + configuration.devices[i].id + "\","
    devicesJSON += "\"deviceName\": \"" + configuration.devices[i].deviceName + "\","
    devicesJSON += "\"lastPosition\": \"" + configuration.devices[i].lastPosition + "\""
    devicesJSON +="}"
  }

  devicesJSON += "]";
}

void handleWifiDisconnect() {
    Serial.println("handleWifiDisconnect ");
    //Serial.println(server.arg("json"));
    server.send(200, "application/json", "{\"message\": \"Disconnecting from WiFi network\"}");
    WiFi.softAP(ap_ssid, ap_password);
}

void wifi_init() {
  delay(1000);
  if(configuration.password !=NULL && sizeof(configuration.password)>0) {
     Serial.println("Found stored WiFi credentials..");
     Serial.println(configuration.ssid);
     Serial.println(configuration.password);
     WiFi.begin(configuration.ssid.c_str(),configuration.password.c_str());
  } else {
     Serial.println("No stored WiFi credentials found.");
     Serial.println("Configuring access point...");

  }
  WiFi.softAP(ap_ssid, ap_password);
  //IPAddress myIP = WiFi.localIP();
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);
  
  server.on("/", handleRoot);
  server.on("/rf_scan", handleRFScanMode);
  server.on("/wifi_connect", HTTP_POST,handleWifiConnect);
  server.on("/wifi_disconnect", handleWifiDisconnect);
  server.on("/device_add", HTTP_POST, handleAddDevice);
  server.on("/device_remove", HTTP_POST, handleRemoveDevice);
  server.on("/device_list", HTTP_GET, handleListDevices);

  server.on("/device_set", HTTP_POST,handleSetDevice);
  server.on("/read_test", HTTP_POST,handleReadTest);
  server.begin();
  
  Serial.println("HTTP server started!");
}

void wifi_handle_client() {
  server.handleClient();
}
