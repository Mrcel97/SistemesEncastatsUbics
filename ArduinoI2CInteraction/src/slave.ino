// Incluimos librería
#include "Wire.h"
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define SLAVE_ADDR 0x04

#define DHTPIN 2
#define DHTTYPE DHT11

#define TRIGGER 6   // Digital pin Ultrasound Trigger sensor
#define ECHO 5   // Digital pin Ultrasound Echo sensor
 
// Inicializamos el sensor DHT11
typedef struct {
  float humidity;
  float temp;
  float acc_x;
  float acc_y;
  float acc_z;
  int distance;
} sensor_data_t;

DHT dht(DHTPIN, DHTTYPE);
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
char accessVar;
sensor_data_t data;

void receive_fn() {
}

void send_fn() { 
  Wire.write((byte*)&data, sizeof(sensor_data_t));
}

void setup() {
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDR);
  dht.begin();

  initialize_accelerometer();
  initialize_ultrasound();
  
  Wire.onReceive(receive_fn);
  Wire.onRequest(send_fn);
}
 
void loop() {
  delay(5000);
  sensors_event_t event;
  accel.getEvent(&event);
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

  Serial.println("----- ACCELEROMETER: -----");
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(" ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" ");
  Serial.println("Z: "); Serial.print(event.acceleration.z); Serial.print(" ");Serial.println("m/s^2 \n");

  Serial.println("----- HUM/TEM SENSOR -----");
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.println(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C \n");
  
  data.humidity = dht.readHumidity();
  data.temp = dht.readTemperature();
  data.acc_x = event.acceleration.x;
  data.acc_y = event.acceleration.y;
  data.acc_z = event.acceleration.z;
  data.distance = d;
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
