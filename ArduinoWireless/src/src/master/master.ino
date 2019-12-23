#include "Wire.h"
#include "LCD.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define SLAVE_ADDR 0x04

SoftwareSerial serial(2, 5);
LCD screenManager;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

typedef struct
{
  float humidity;
  float temp;
  int distance;
} sensor_data_t;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  initialize_accelerometer();
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
  sensors_event_t event;
  accel.getEvent(&event);
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

  float acc_x = event.acceleration.x;
  float acc_y = event.acceleration.y;
  float acc_z = event.acceleration.z;
  
  String acc = "Acc: " + String(acc_x) + "," + String(acc_y) + "," + String(acc_z);
  acc.toCharArray(acceleration, 64);
  screenManager.print(1, 40, acceleration);

  String dist = "Distance: " + String(data.distance) + " cm";
  dist.toCharArray(distance, 64);
  screenManager.print(1, 30, distance);
}

void initialize_accelerometer() {
  if(!accel.begin()) {
   /* ADXL345 not detected... */
   Serial.println("No ADXL345 detected...");
   while(1);
  }
  
  /* Different ranges for ADX345 sensor */
  accel.setRange(ADXL345_RANGE_16_G);
  // displaySetRange(ADXL345_RANGE_8_G);
  // displaySetRange(ADXL345_RANGE_4_G);
  // displaySetRange(ADXL345_RANGE_2_G);
}
