#include <Arduino.h>
// #include <SPI.h> // Don't strictly need this, but PIO chokes if it's not included here.
#include <Wifi.h>
#include <WiFiClient.h>
//#include <Wire.h>
#include "SSD1306Wire.h" 
#include "time.h"

SSD1306Wire display(0x3c, 5, 4);  // ADDRESS, SDA, SCL

// put the WIFI credentials in wificreds.h, in the same directory as buttonthing.cpp (this file) with the lines:
// const char* ssid = "WIFI SSID";
// const char* password = "WIFI SECRET PASSWORD";
// (uncommented, of course)
#include "wificreds.h"

unsigned int myMins = 0;
unsigned int myHRs = 0;
unsigned int brightness = 3;
unsigned int secsElapsed = 0;

// Grab the time from this server:
const char* ntpServer = "time.apple.com";

/* GMT offset (in seconds)
   For UTC -5.00 : -5 * 60 * 60 : -18000
   For UTC +1.00 : 1 * 60 * 60 : 3600
   For UTC +0.00 : 0 * 60 * 60 : 0
*/
const long gmtOffset_sec = -18000; // EST

// DST offset (on or off really...)
//const int daylightOffset_sec = 3600;
const int daylightOffset_sec = 0;

int lastMin = 99; // first run should always be != to the current minute
const int led=13; // do we still need this?

void updateDisplay(int myHRs, int myMins);
void printLocalTime();

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  digitalWrite(led, 1);

    // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Start WiFi and");
  display.drawString(0, 16, "get the time...");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0,40,"v1.0 2020-02-07");
  display.display();

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  // Set ESP to be a WiFi client 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
  Serial.println();
  Serial.println();
  Serial.println("wemosclock - 2020-02-07");
  Serial.println("Screw you Ahmed, you poser.");
  Serial.println();
  digitalWrite(led, 0);
  
  delay(1000);
  display.clear();
  display.display();
  delay(100); 
}

void loop() {
  secsElapsed = millis() / 1000;

  // Leftover diagnostic code - simply displays the time elapsed since power-on.
  //myMins = secsElapsed / 60;
  //myHRs = myMins / 60;
  //updateDisplay(myHRs, myMins);
  
  delay(1000);
  printLocalTime();
  
}

void updateDisplay(int myHRs, int myMins){
  String timeString;
  if(myMins != lastMin) {  // Avoid hammering the display - it does exhibit a brief flicker on .display() when redrawing

  // Let's force leading zeros while building the display string
  if(myHRs < 10) timeString = "0" + String(myHRs);
  else timeString = String(myHRs);

  timeString = timeString+":";

  if(myMins < 10) timeString = timeString + "0" + String(myMins);
  else timeString = timeString + String(myMins);

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 0, timeString);
  display.display();

  //Serial.println(timeString);
  }

} // end of updateDisplay()

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  updateDisplay(timeinfo.tm_hour, timeinfo.tm_min);
}
