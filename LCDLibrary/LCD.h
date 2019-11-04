/**********************************************
LCD Libary
Guillem Orellana, Jordi Onrubia, Marcel Ortiz
LCD - Activity
4-11-2019
**********************************************/

#ifndef LCD_h
#define LCD_h
#include "SoftwareSerial.h"
class LCD
{
public:
  LCD();
  void begin(SoftwareSerial *serialSoft);
  void print(int posX, int posY, char Str[78]);
  void clearScreen();
  void setX(int posX);
  void setY(int posY);
  void ressetXY();
  void sPixel(int x, int y);
  void clPixel(int x, int y);
  void line(int x1, int y1, int x2, int y2, int ink);
  void rectangle(int x1, int y1, int x2, int y2);
  void circle(int x, int y, int rad);

private:
  SoftwareSerial *serial;
};

#endif