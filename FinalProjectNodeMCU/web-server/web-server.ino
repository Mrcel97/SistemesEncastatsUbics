#include "ESP8266WiFi.h"
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DHT.h>

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

#ifndef STASSID
#define STASSID "Arduino"
#define STAPSK  "guirado.ino"
#endif

#define DHTPIN D4
#define DHTTYPE DHT11

#define TRIGGER D1 // Digital pin Ultrasound Trigger sensor
#define ECHO D2    // Digital pin Ultrasound Echo sensor

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

DHT dht(DHTPIN, DHTTYPE);

float humidity = 0, temperature = 0, distance = 0;

void initialize_ultrasound()
{
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
}


void setup(void) {
  Serial.begin(115200);
  
  dht.begin();
  delay(750);
  
  initialize_ultrasound();
  delay(750);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", []() {
      server.send(200, 
                  "text/plain", 
                  String(humidity) + "," + String(temperature) + "," + String(distance));
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found"); 
  });

  server.begin();
  Serial.println("HTTP server started");
}

int getDistance() 
{
  long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(3); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(12); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  return distance; 
}

void loop(void) 
{
  server.handleClient();
  MDNS.update();

  delay(500);
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  distance = getDistance();
}
