#include "Wire.h"
#include "LCD.h"

#define SLAVE_ADDR 0x04

SoftwareSerial serial(2, 5);
LCD screenManager;

typedef struct
{
  float humidity;
  float temp;
  float acc_x;
  float acc_y;
  float acc_z;
  int distance;
} sensor_data_t;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  screenManager.begin(&serial);
}

void loop()
{
  char humidity[64];
  char temperature[64];
  char acceleration[64];
  char distance[64];

  sensor_data_t data;

  delay(5000);

  // Request data to slave
  Wire.requestFrom(SLAVE_ADDR, sizeof(sensor_data_t));
  Wire.readBytes((byte *)&data, sizeof(sensor_data_t));

  screenManager.clearScreen();

  String hum = "Humidity: " + String(data.humidity) + " %";
  hum.toCharArray(humidity, 64);
  screenManager.print(1, 60, humidity);

  String temp = "Temperature: " + String(data.temp) + " *C";
  temp.toCharArray(temperature, 64);
  screenManager.print(1, 50, temperature);

  String acc = "Acc: " + String(data.acc_x) + "," + String(data.acc_y) + "," + String(data.acc_z);
  acc.toCharArray(acceleration, 64);
  screenManager.print(1, 40, acceleration);

  String dist = "Distance: " + String(data.distance) + " cm";
  dist.toCharArray(distance, 64);
  screenManager.print(1, 30, distance);
}
