#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "HOOOOO";        // your network SSID (name)
const char* password = "12345678";  // your network password

WiFiClient client;
// the Wifi radio's status
int status = WL_IDLE_STATUS;


unsigned long myChannelNumber = 1;
const char* myWriteAPIKey = "OZRY2HO5WQDRIY2N";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;


// Alcohol sensor
#define MQ3 0

int alcohol;

void setup() {
  Serial.begin(115200);  //Initialize serial
  InitWiFi();
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  if ((millis() - lastTime) < timerDelay) return;

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  // Get a new alcohol reading
  alcohol = analogRead(MQ3);

  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, alcohol, myWriteAPIKey);

  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  lastTime = millis();
}

void InitWiFi() {
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if (status != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}