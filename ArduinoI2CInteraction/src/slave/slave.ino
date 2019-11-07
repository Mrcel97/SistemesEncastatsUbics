#include "Wire.h"
#include <DHT.h>

#define SLAVE_ADDR 0x04

#define DHTPIN 2
#define DHTTYPE DHT11

#define TRIGGER 6   // Digital pin Ultrasound Trigger sensor
#define ECHO 5   // Digital pin Ultrasound Echo sensor
 
// Inicializamos el sensor DHT11
typedef struct {
  float humidity;
  float temp;
  int distance;
} sensor_data_t;

DHT dht(DHTPIN, DHTTYPE);
char accessVar;
sensor_data_t data;

void send_fn() {
  Serial.println("--request found--");
  Wire.write((byte*)&data, sizeof(sensor_data_t));
}

void setup() {
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDR);
  dht.begin();

  initialize_ultrasound();
  
  Wire.onRequest(send_fn);
}
 
void loop() {
  delay(5000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  long echoTime; // Time until echo is reached
  long d; // Distance (cm)
  ultrasoundDigitalWrite();
  echoTime = pulseIn(ECHO, HIGH); // Get the width of the pulse
  d = echoTime/59; // Scale time to distance in cm

  clearTerminal();
  Serial.println("----- ULTRASOUND -----");
  Serial.print("Distancia: ");
  Serial.print(d); //Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println("\n");

  Serial.println("----- HUM/TEM SENSOR -----");
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.println(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C \n");
  
  data.humidity = dht.readHumidity();
  data.temp = dht.readTemperature();
  data.distance = d;
}

void initialize_ultrasound() {
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIGGER, LOW);
}

void ultrasoundDigitalWrite() {
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
}

void clearTerminal() {
  for(int i = 0; i<30; i++) {
    Serial.println();
  }
}
