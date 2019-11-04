/**********************************************
LCD Libary
Guillem Orellana, Jordi Onrubia, Marcel Ortiz
LCD - Activity
4-11-2019
**********************************************/
#include "Arduino.h"
#include "LCD.h"

LCD::LCD() {}

void LCD::begin(SoftwareSerial *serialSoft) {
  this->serial = serialSoft;
  serial->begin(115200);
}

void LCD::print(int posX, int posY, char Str[78]) {
  this->setX(byte(posX));
  this->setY(byte(posY));
	serial->print(Str);
  this->ressetXY();
}


void LCD::clearScreen() {
  serial->write(0x7C);
  serial->write(byte(0));
}

void LCD::setX(int posX) {
  serial->write(0x7C);
  serial->write(0x18);
  serial->write(byte(posX));
}

void LCD::setY(int posY) { 
  serial->write(0x7C);
  serial->write(0x19);
  serial->write(byte(posY));
}

void LCD::ressetXY() {
  this->setX(byte(1));
  this->setY(byte(63));
}

void LCD::sPixel(int x, int y) {
  serial->write(0x7C);
  serial->write(0x10);
  serial->write(byte(x));
  serial->write(byte(y));
  serial->write(0x01);
  delay(10);
}

void LCD::clPixel(int x, int y) {
  serial->write(0x7C);
  serial->write(0x10);
  serial->write(byte(x));
  serial->write(byte(y));
  serial->write(byte(0));
  delay(10);
}

void LCD::line(int x1, int y1, int x2, int y2, int ink) {
  serial->write(0x7C);
  serial->write(0x0C); 
  serial->write(byte(x1));
  serial->write(byte(y1));
  serial->write(byte(x2));
  serial->write(byte(y2));
  serial->write(byte(ink));
  delay(10);
}

void LCD::rectangle(int x1, int y1, int x2, int y2, int ink) {
  serial->write(0x7C);
  serial->write(0x0F);
  serial->write(byte(x1));
  serial->write(byte(y1));
  serial->write(byte(x2));
  serial->write(byte(y2));
  // serial->write(byte(ink));
  delay(10);
}

void LCD::circle(int x, int y, int rad) {
  serial->write(0x7C);
  serial->write(0x03); 
  serial->write(byte(x));
  serial->write(byte(y));
  serial->write(byte(rad));
  serial->write(0x01);
  delay(10);
}
