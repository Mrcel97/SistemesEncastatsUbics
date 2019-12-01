#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Wire.h"

#ifndef STASSID
#define STASSID "Arduino"
#define STAPSK  "guirado.ino"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

typedef struct {
  float humidity;
  float temperature;
  float distance;
} sensor_data_t;

#define SLAVE_ADDR 0x04

sensor_data_t data;

void send_fn() 
{
  Serial.println("--request found--");
  Wire.write((byte*)&data, sizeof(sensor_data_t));
}

void setup()
{
  Serial.begin(115200);
  Wire.begin(SLAVE_ADDR);
    
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

  Wire.onRequest(send_fn);
}
 
void loop()
{
   String res = request("http://192.168.43.89/");

   data.humidity = getValue(res, ',', 0).toFloat();
   data.temperature = getValue(res, ',', 1).toFloat();
   data.distance = getValue(res, ',', 2).toFloat();
   
   Serial.println("REQUESTING...");
   Serial.println(res);
   delay(5000);
}

String request(String url) 
{
  HTTPClient http;
  WiFiClient client;

  if (http.begin(client, url)) 
  {
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();
 
      if (httpCode > 0) 
      {
         Serial.printf("[HTTP] GET... code: %d\n", httpCode);
 
         if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
         {
            String payload = http.getString();
            return payload;
         }
      }
      else 
      {
         Serial.printf("[HTTP] GET... failed,\n");
      }
 
      http.end();
   }
   else 
   {
      Serial.printf("[HTTP} Unable to connect\n");
   }
   return "";
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
