#include <Servo.h>
#include "Touchpadlib.h"

bool debug = false;
Servo servoX;
Servo servoY;
Touchpadlib touchpad(A1, A0, A2, A3);

int x, y, lastX, lastY;

float targetX = 0;
float targetY = 0;

float regulatingX = 90;
float regulatingY = 90;

float kpX = 0.3;
float kpY = 0.3;

float kdX = 0.12;
float kdY = 0.12;

float ix = -20800; //not exactly 90 degree mounted servowheels or similar
float kiX = 0.001;

float iy = -27000; //not exactly 90 degree mounted servowheels or similar
float kiY = 0.001;

unsigned long now;
unsigned long lastTime;

//commands:
bool cmdComplete = false;
String cmdString = "";

unsigned long wait = 20;

float smoothX = 0;
float smoothY = 0;
float targetDistMax = 40;

void setup() {
  Serial.begin(9600);

  servoX.attach(9);
  servoY.attach(8);

  servoX.write(regulatingX);
  servoY.write(regulatingY);
}

//circle program:
float alpha = 0;
float radius = 30;
bool circleDir = true;
bool circleActive = false;
float deltaAlpha = 15;

//rectangle program:
bool rectActive = false;
int currentCorner = 0; //coords -> 0: 50,50; 1: -50,50; 2: -50,-50; 3: 50,-50

void targetReached() {
  if (circleActive) {
    if (circleDir) {
      alpha += deltaAlpha;
    } else {
      alpha -= deltaAlpha;
    }

    if (alpha >= 360) {
      alpha = 0;
    }
    targetX = cos(alpha / 180 * PI) * radius;
    targetY = sin (alpha / 180 * PI) * radius;

    if (debug) {
      Serial.print("alpha: ");
      Serial.println(alpha);
    }
  } else if (rectActive) {
    if (currentCorner == 0) {
      targetX = -30; targetY = 30;
    }
    if (currentCorner == 1) {
      targetX = -30; targetY = -30;
    }
    if (currentCorner == 2) {
      targetX = 30; targetY = -30;
    }
    if (currentCorner == 3) {
      targetX = 30; targetY = 30;
    }
    currentCorner++;
    if (currentCorner > 3) currentCorner = 0;
  }
}

void checkCommands() {
  if (cmdComplete) { //ie. "w 150" for "wait 150 ms"
    String key = cmdString.substring(0, 2); //die ersten beiden zeichen ie. "w "
    String val = cmdString.substring(2); //zeichen ab pos. 2 ie. "150"

    if (key == "w ") {
      wait = val.toInt();
    }
    if (key == "tx") {
      targetX = val.toInt();
    }
    if (key == "ty") {
      targetY = val.toInt();
    }
    if (key == "p ") {
      kpX = val.toFloat();
    }
    if (key == "d ") {
      kdX = val.toFloat();
    }
    if (key == "ix") {
      kiX = val.toFloat();
    }
    if (key == "iy") {
      kiY = val.toFloat();
    }
    if (key == "sx") {
      smoothX = val.toFloat();
    }
    if (key == "sy") {
      smoothY = val.toFloat();
    }
    if (key == "td") {
      targetDistMax = val.toFloat();
    }
    if (key == "sw") {
      circleDir = !circleDir;
    }
    if (key == "ca") {
      circleActive = !circleActive;
      if (debug) {
        Serial.print("circle active: ");
        Serial.println(circleActive);
      }
    }
    if (key == "da") {
      deltaAlpha = val.toFloat();
    }
    if (key == "ra") {
      rectActive = !rectActive;
    }

    cmdComplete = false;
    cmdString = "";
  }
}

void loop() {
  checkCommands();

  touchpad.getXY(x, y);

  Serial.print(targetX);
  Serial.print("\t");
  Serial.print(targetY);
  Serial.print("\t");
  touchpad.serialPrintXY();
  Serial.println();

  float deltaXprev = x - lastX;
  float deltaYprev = y - lastY;

  float deltaXtarget = x - targetX;
  float deltaYtarget = y - targetY;
  
  //  Serial.print("delta: ");
  //  Serial.print(deltaXtarget);
  //  Serial.print("\t");
  //  Serial.println(deltaYtarget);
  float targetDist = sqrt(sq(deltaXtarget) + sq(deltaYtarget));
  //  Serial.print("delta: ");
  //  Serial.println(targetDist);
  if (targetDist < targetDistMax) {
    targetReached();
  }

  now = millis();
  //speed:
  float vx = 1000 * deltaXprev / (now - lastTime); //[mm/s]
  float vy = 1000 * deltaYprev / (now - lastTime); //[mm/s]

  //proportional:
  float pX = deltaXtarget * kpX;
  float pY = deltaYtarget * kpY;

  //differential:
  float dX = vx * kdX;
  float dY = vy * kdY;

  //integral:
  ix += x - targetX;
  iy += y - targetY;

  if (debug) {
    Serial.print("v: ");
    Serial.print(vx);
    Serial.print("\t");
    Serial.println(vy);

    Serial.print("proportional: ");
    Serial.print(pX);
    Serial.print("\t");
    Serial.println(pY);

    Serial.print("differential: ");
    Serial.print(dX);
    Serial.print("\t");
    Serial.println(dY);

    Serial.print("integral: ");
    Serial.print(ix);
    Serial.print("\t");
    Serial.println(iy);
  }

  //calculate control variables:
  float regulatingXnew = 90 + pX + dX + ix * kiX;
  float regulatingYnew = 90 + pY + dY + iy * kiY;

  //control the servos:
  regulatingX = (regulatingX * smoothX + regulatingXnew) / (smoothX + 1);
  regulatingY = (regulatingY * smoothY + regulatingYnew) / (smoothY + 1);

  if (debug) {
    Serial.print("regulating: ");
    Serial.print(regulatingX);
    Serial.print("\t");
    Serial.println(regulatingY);
  }

  servoX.write(regulatingX);
  servoY.write(regulatingY);

  lastX = x;
  lastY = y;
  lastTime = now;

  delay(wait);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      cmdComplete = true;
    } else {
      cmdString += inChar;
    }
  }
}

