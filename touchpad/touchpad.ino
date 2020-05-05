#include "Touchpadlib.h"

Touchpadlib touchpad(A1, A0, A2, A3);
int x, y;

void setup() {
  Serial.begin(9600);
}

void loop() {
  touchpad.getXY(x, y);
  touchpad.serialPrintXY();
  delay(100);
}

