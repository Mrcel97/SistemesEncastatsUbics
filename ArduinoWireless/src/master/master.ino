// #include "Wire.h"
#include "LCD.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "SoftwareSerial.h"

#define SLAVE_ADDR 0x04

SoftwareSerial serial(2, 3);
LCD screenManager;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup()
{
  Serial.begin(9600);
  serial.begin(115200);
  initialize_accelerometer();
  screenManager.begin(&serial);
}

void loop()
{
  sensors_event_t event;

  // Request data to slave
  while(!Serial.available());
  
  accel.getEvent(&event);

  String message = Serial.readString();
  
  screenManager.clearScreen();
      
  String hum = "Humidity: " + getValue(message, ',', 0) + " %";
  print(1, 60, hum);

  String temp = "Temperature: " + getValue(message, ',', 1) + " *C";
  print(1, 50, temp);

  float acc_x = event.acceleration.x;
  float acc_y = event.acceleration.y;
  float acc_z = event.acceleration.z;

  String acc = "Acc: " + String(acc_x) + "," + String(acc_y) + "," + String(acc_z);
  print(1, 40, acc);

  String dist = "Distance: " + getValue(message, ',', 2) + " cm";
  print(1, 30, dist);
}

void print(int x, int y, String msg)
{
  char buffer[64];
  msg.toCharArray(buffer, 64);
  screenManager.print(x, y, buffer);
}

void initialize_accelerometer()
{
  if (!accel.begin())
  {
    /* ADXL345 not detected... */
    Serial.println("No ADXL345 detected...");
    while (1)
      ;
  }

  accel.setRange(ADXL345_RANGE_16_G);
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
