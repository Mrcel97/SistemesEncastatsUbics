#include <DHT.h>
#include "SoftwareSerial.h"

#define DHTPIN 4
#define DHTTYPE DHT11

#define TRIGGER 6 // Digital pin Ultrasound Trigger sensor
#define ECHO 5    // Digital pin Ultrasound Echo sensor

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial xbee(2, 3);

void setup()
{
  Serial.begin(9600);
  xbee.begin(9600);
  
  dht.begin();
  initialize_ultrasound();
}

int getDistance() 
{
  long echoTime; // Time until echo is reached
  long d;        // Distance (cm)
  ultrasoundDigitalWrite();
  echoTime = pulseIn(ECHO, HIGH); // Get the width of the pulse
  d = echoTime / 59;              // Scale time to distance in cm
  return d; 
}

void loop()
{
  Serial.println("Waiting for master..");
  
  while(!xbee.available()); // Wait for master request
  
  if (xbee.readString() == "Request")
  {
    // Master is requesting info
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float distance = getDistance();
    
    // Return data to master
    Serial.println("Sending data...");
    Serial.println(String(h) + "," + String(t) + "," + String(distance)); 
    xbee.print(String(h) + "," + String(t) + "," + String(distance)); 
  }
}

void initialize_ultrasound()
{
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIGGER, LOW);
}

void ultrasoundDigitalWrite()
{
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
}
