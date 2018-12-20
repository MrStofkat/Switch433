/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/

#include <ESP8266WiFi.h>


const char* WEATHER_API_URL = "api.openweathermap.org";
const char* streamId   = "....................";
const char* privateKey = "....................";
String WEATHER_LOCATION = "Amsterdam,nl";
String WEATHER_API_KEY = "a5e048469742be59ef9836e97e6afaf7";
void weather_setup() {

}


void weather_getCurrent() {

  Serial.print("connecting to ");
  Serial.println(WEATHER_API_URL);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(WEATHER_API_URL, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = String("/data/2.5/weather/") + String("?q=") + WEATHER_LOCATION + String("&appid=") + WEATHER_API_KEY;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + WEATHER_API_URL + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  String line;
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    line = client.readStringUntil('\r');
    Serial.println(line);
//    pendingDeviceName = root["name"].as<String>();

  }
  StaticJsonBuffer<1200> jsonBuffer;
  //"{"coord":{"lon":4.89,"lat":52.37},"weather":[{"id":741,"main":"Fog","description":"fog","icon":"50d"}],"base":"stations","main":{"temp":276.19,"pressure":1009,"humidity":100,"temp_min":275.15,"temp_max":277.15},"visibility":200,"wind":{"speed":3.1,"deg":140},"clouds":{"all":90},"dt":1544966820,"sys":{"type":1,"id":1524,"message":0.0041,"country":"NL","sunrise":1544946315,"sunset":1544974037},"id":2759794,"name":"Amsterdam","cod":200}";
  JsonObject& object = jsonBuffer.parseObject(line);
  int weatherType = object["weather"][0]["id"];
  Serial.print("Weather type: ");
  Serial.println(weatherType);
  if (weatherType < 600) {
    //Rain
    Serial.println("Rain");
    led_blue();
  } else if (weatherType >= 600 && weatherType < 700) {
    //Snow
    Serial.println("Snow");
    led_christmasMode = true;
  } else if (weatherType >= 700 && weatherType < 800) {
    //Weird atmosphere
    Serial.println("Weird");
    led_red();
  } else if (weatherType == 800 || weatherType == 801) {
    //Clear
    Serial.println("Clear");
    led_yellow();
  } else if((weatherType >= 701 && weatherType < 721) || weatherType > 801 ) {
    //clouds
    Serial.println("Clouds");
    led_white();
  }
    
  Serial.println("closing connection");
}

