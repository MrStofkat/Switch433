/*
* Scan for RF signals and map them for later usage
*/
#include <NewRemoteReceiver.h>
#include <RemoteReceiver.h>
#include "Configuration.h"

int LED_TIMEOUT = 200;
int mainLoopCounter = 0;
int WEATHER_INTERVAL = 200;
int YELLOW_LED = D5;
int RED_LED = D6;
int GREEN_LED = D7;
int WHITE_LED = D4;

boolean led_christmasMode = false;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); 
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(WHITE_LED, LOW);
  Serial.begin(115200);//for debugging purposes.
  config_init();
  wifi_init();
  rf_init();
}

void led_red() {
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(WHITE_LED, LOW);
  digitalWrite(LED_BUILTIN, HIGH); 
}
void led_green() {
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(WHITE_LED, LOW);
  digitalWrite(LED_BUILTIN, HIGH); 
}
void led_blue() {
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(WHITE_LED, LOW);
  digitalWrite(LED_BUILTIN, LOW); 
}

void led_yellow() {
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(WHITE_LED, LOW);
  digitalWrite(LED_BUILTIN, HIGH); 
}

void led_white() {
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(WHITE_LED, HIGH);
  digitalWrite(LED_BUILTIN, HIGH); 
}

void led_christmasModePattern() {
  led_red();
  delay(LED_TIMEOUT);
  led_green();
  delay(LED_TIMEOUT);    
  led_blue();
  delay(LED_TIMEOUT);
  led_white();
  delay(LED_TIMEOUT);
}

void loop() {
  
  wifi_handle_client();
  if (led_christmasMode) {
    led_christmasModePattern();
  }
  
  if (mainLoopCounter % WEATHER_INTERVAL == 0) {
    led_christmasMode= true;
    weather_getCurrent();
  }
  
  mainLoopCounter++;
  delay(50);

}

      
