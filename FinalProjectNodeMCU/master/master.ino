#include "Wire.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define SLAVE_ADDR 0x04

SoftwareSerial serial(2, 3);

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

typedef struct {
  float acc_x;
  float acc_y;
  float acc_z;

  float humidity;
  float temperature;

  float distance;
} sensor_data_t;

sensor_data_t data;

void send_fn() {
  // Raspberry requesting info
  Wire.write((byte*)&data, sizeof(sensor_data_t));
}

void setup()
{
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDR);
  initialize_accelerometer();
}

void loop()
{
  sensors_event_t event;

  // Request data to slave
  while(!Serial.available());
  
  accel.getEvent(&event);

  String message = Serial.readString();
        
  data.acc_x = event.acceleration.x;
  data.acc_y = event.acceleration.y;
  data.acc_z = event.acceleration.z;

  data.humidity = getValue(message, ',', 0).toFloat();
  data.temperature = getValue(message, ',', 1).toFloat();
  data.distance = getValue(message, ',', 2).toFloat();
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
