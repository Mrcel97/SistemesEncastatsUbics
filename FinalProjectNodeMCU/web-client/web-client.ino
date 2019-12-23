#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <SparkFun_ADXL345.h> 
#include "LCD.h"

#ifndef STASSID
#define STASSID "Arduino"
#define STAPSK  "guirado.ino"
#endif
#define SERVER "http://192.168.43.88/"

const char* ssid = STASSID;
const char* password = STAPSK;

typedef struct {
  float humidity;
  float temperature;
  float distance;
} sensor_data_t;

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

#define SDA_PIN D1
#define SCL_PIN D2


ADXL345 adxl = ADXL345();
sensor_data_t data;

void clearScreen() 
{
  Serial.write(0x7C);
  Serial.write(byte(0));
}  

void print(int posX, int posY, String str) 
{
  char buff[128];
  str.toCharArray(buff, 128);
  
  setX(byte(posX));
  setY(byte(posY));
  Serial.print(buff);
  resetXY();
}

void setX(int posX) 
{
  Serial.write(0x7C);
  Serial.write(0x18);
  Serial.write(byte(posX));
}

void setY(int posY) 
{ 
  Serial.write(0x7C);
  Serial.write(0x19);
  Serial.write(byte(posY));
}

void resetXY() 
{
  setX(byte(1));
  setY(byte(63));
}


void setup()
{
  delay(500);
  
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  resetXY();
  clearScreen();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  print(10, 60, "Connected to " + String(ssid));
  print(10, 40, "IP address: " + WiFi.localIP().toString());

  adxl.powerOn();            
  adxl.setRangeSetting(16);
  delay(10000);
}
 
void loop()
{
  clearScreen();
  
  String res = request(SERVER);
  delay(10);
   
  data.humidity = getValue(res, ',', 0).toFloat();
  data.temperature = getValue(res, ',', 1).toFloat();
  data.distance = getValue(res, ',', 2).toFloat();  
  Serial.println("REQUESTING...");
  Serial.println(res);

  Serial.println("Acelerometer...");

  int x, y, z;
  adxl.readAccel(&x, &y, &z);  
  Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.println(z); 
  
  delay(5000);
}

String request(String url) 
{
  HTTPClient http;
  WiFiClient client;

  if (http.begin(client, url)) //Iniciar conexión
  {
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();  // Realizar petición
 
      if (httpCode > 0) 
      {
         Serial.printf("[HTTP] GET... code: %d\n", httpCode);
 
         if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();   // Obtener respuesta
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
