#include "Arduino.h"
#include "Touchpadlib.h"

Touchpadlib::Touchpadlib() {
  int pinX1 = A1; //yellow
  int pinX2 = A0; //orange
  int pinY1 = A2; //green
  int pinY2 = A3; //red
}

Touchpadlib::Touchpadlib(int x1, int x2, int y1, int y2) {
  int pinX1 = x1;
  int pinX2 = x2;
  int pinY1 = y1;
  int pinY2 = y2;
}

void Touchpadlib::serialPrintXY() {
  Serial.print(x);
  Serial.print("\t");
  Serial.println(y);
}

void Touchpadlib::updateX() {
  pinMode(pinY1, INPUT);
  pinMode(pinY2, INPUT);
  pinMode(pinX1, OUTPUT);
  pinMode(pinX2, OUTPUT);

  digitalWrite(pinX1, LOW);
  digitalWrite(pinX2, LOW);
  digitalWrite(pinX1, HIGH);
  delay(10);

  x = analogRead(pinY1);
  x = map(x, 53, 920, 0, 100);
  if (x < 0) x = 0;
  if (x > 100) x = 100;
}

void Touchpadlib::updateY() {
  pinMode(pinY1, OUTPUT);
  pinMode(pinY2, OUTPUT);
  pinMode(pinX1, INPUT);
  pinMode(pinX2, INPUT);

  digitalWrite(pinY1, LOW);
  digitalWrite(pinY2, LOW);
  digitalWrite(pinY1, HIGH);
  delay(10);

  y = analogRead(pinX2);
  y = map(y, 121, 898, 0, 100);
  if (y < 0) y = 0;
  if (y > 100) y = 100;
}

void Touchpadlib::getXY(int& x1, int& y1) {
  updateX();
  updateY();

  x1 = x;
  y1 = y;
}

