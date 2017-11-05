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
const char *ssid = "Stofkat Home Automation";
const char *password = "thereisnospoon";

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
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
    const char * ssid = root["ssid"];
    const char * password = root["password"];
    Serial.println(password);
    Serial.println("handleWifiConnect END");
    WiFi.begin(ssid,password);
}


void handleSetDevice() {
    Serial.println("handleSetDevice ");
    Serial.println(server.arg("plain"));
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));
    int deviceID = root["deviceID"];
    boolean state = root["state"];
    rf_sendSignal(deviceID, state);
 }

void handleAddDevice() {
    Serial.println("handleWifiConnect ");
    Serial.println(server.arg("plain"));
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(server.arg("json"));
    server.send(200, "application/json", "{\"message\": \"Scanning for new RF433 signal\"}");
    hasPendingDevice = true;
    pendingDeviceName = root["name"];
    //addDevice("",2,2);
}

void handleWifiDisconnect() {
    Serial.println("handleWifiDisconnect ");
    //Serial.println(server.arg("json"));
    server.send(200, "application/json", "{\"message\": \"Disconnecting from WiFi network\"}");
    WiFi.softAP(ssid, password);
}

void wifi_module_start() {
  delay(1000);
  Serial.println("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
  //WiFi.begin("Why not Zoidberg?", "br00dr00ster");
  //IPAddress myIP = WiFi.localIP();
 
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/rf_scan", handleRFScanMode);
  server.on("/wifi_connect", HTTP_POST,handleWifiConnect);
  server.on("/wifi_disconnect", handleWifiDisconnect);
  server.on("/device_add", HTTP_POST, handleAddDevice);
  server.on("/device_set", HTTP_POST,handleSetDevice);

  server.begin();
  Serial.println("HTTP server started");
}

void wifi_module_handle_client() {
  server.handleClient();
}
