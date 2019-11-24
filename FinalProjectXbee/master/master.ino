#include "Wire.h"
#include <Adafruit_Sensor.h>
#include "SoftwareSerial.h"

#define SLAVE_ADDR 0x04

uint8_t value1 = 4, value2 = 2;

SoftwareSerial xbee(2,3);
bool tables_on = false;

typedef struct {
  float humidity;
  float temperature;

  float distance;
} sensor_data_t;

sensor_data_t data;

void send_fn() {
  // Raspberry requesting info

  if (tables_on) {
    Serial.print("=");
    Serial.println((uint8_t)(value1*value2));
    Wire.write((uint8_t)(value1*value2));
  } else {
    Serial.println("Sending data...");
    Serial.print("Struct contains:\n");
    Serial.print("HUM: ");
    Serial.println(data.humidity);
    Serial.print("TMP: ");
    Serial.println(data.temperature);
    Serial.print("DST: ");
    Serial.println(data.distance);
    Serial.print("");
    Wire.write((byte*)&data, sizeof(sensor_data_t));
    /*
     * Byte-to-Byte
    uint8_t* d = (uint8_t*)&data;
    int s = sizeof(d);
    for (int i = 0; i < s; i++)
      Wire.write(*d++); 
    */
  }
}

void setup()
{
  Serial.begin(9600);
  xbee.begin(9600);
  Wire.begin(SLAVE_ADDR);
  Wire.onRequest(send_fn);
}

void loop()
{
  sensors_event_t event;

  // Request data to slave
  while(!xbee.available());

  String message = xbee.readString();

  if (tables_on) {
    Serial.print(value1);
    Serial.print("x");
    Serial.print(value2);
  } else {
    data.humidity = getValue(message, ',', 0).toFloat();
    data.temperature = getValue(message, ',', 1).toFloat();
    data.distance = getValue(message, ',', 2).toFloat();
  }
  Serial.println("Loop\n");
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
