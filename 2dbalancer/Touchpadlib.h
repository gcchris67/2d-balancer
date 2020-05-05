#ifndef Touchpadlib_h
#define Touchpadlib_h

#include "Arduino.h"

class Touchpadlib {
  private:
    int x, y;
    int pinX1 = A1; //yellow
    int pinX2 = A0; //orange
    int pinY1 = A2; //green
    int pinY2 = A3; //red
    void updateX();
    void updateY();

  public:
    Touchpadlib();
    Touchpadlib(int x1, int x2, int y1, int y2);
    void serialPrintXY();
    void getXY(int& x1, int& y1);
};

#endif
