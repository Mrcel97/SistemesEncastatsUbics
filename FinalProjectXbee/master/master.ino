#include "Wire.h"
#include <Adafruit_Sensor.h>
#include "SoftwareSerial.h"

#define SLAVE_ADDR 0x04

SoftwareSerial xbee(2, 3);

typedef struct
{
  float humidity;
  float temperature;

  float distance;
} sensor_data_t;

sensor_data_t data;

void send_fn()
{

  Serial.println("Sending data...");
  Serial.print("Struct contains:\n");
  Serial.print("HUM: ");
  Serial.println(data.humidity);
  Serial.print("TMP: ");
  Serial.println(data.temperature);
  Serial.print("DST: ");
  Serial.println(data.distance);
  Serial.print("");
  Wire.write((byte *)&data, sizeof(sensor_data_t));
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
  delay(5000);
  
  // Request data to slave
  xbee.print("Request");
  
  // Wait until slave sends us the data
  while (!xbee.available())
    ;

  String message = xbee.readString();
  
  Serial.println(message);

  data.humidity = getValue(message, ',', 0).toFloat();
  data.temperature = getValue(message, ',', 1).toFloat();
  data.distance = getValue(message, ',', 2).toFloat();
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
