import g4p_controls.*; // Need G4P library
import processing.serial.*;

Serial serial;
PadView padView;

void setup() {
  size(800, 470, JAVA2D);
  createGUI();

  padView = new PadView();
  view1.addListener(padView);

  noSmooth(); //otherwise the text is anti-aliased (horrible)

  //switchCircleDir.setEnabled(false);

  initSerial();
}

void setTarget(int x, int y) {
  if (serial != null) {
    serial.write("tx" + x + "\n");
    delay(100);
    serial.write("ty" + y + "\n");
  }
}

void initSerial() {
  boolean serialFound = Serial.list().length > 0;
  if (serialFound) {
    String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
    serial = new Serial(this, portName, 9600);
  }
}

void switchCircleDir() {
  if (serial != null) {
    serial.write("sw\n");
  }
}

void mode(String modeName) {
  if (serial == null) {
    return;
  }

  if (modeName.equals("circle")) {
    serial.write("ca\n");
  } else if (modeName.equals("eight")) {
    serial.write("ca\n");
  } else {
    serial.write("ca\n");
  }
}

void targetOrg() {
  if (serial != null) {
    serial.write("tx0\n");
    delay(100);
    serial.write("ty0\n");
  }
}

void draw() {
  background(200);
}
