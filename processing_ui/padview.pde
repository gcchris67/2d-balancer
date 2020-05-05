public class PadView extends GViewListener { //<>//

  float x, y, targetX, targetY;
  float scale = 3.0;

  PadView() {
  }

  public void update() {
    PGraphics v = getGraphics();
    v.beginDraw();

    v.background(30);
    v.stroke(255); //white
    v.fill(255);
    //v.text(millis(), v.width-50, v.height-5);

    String serialData = "";
    if (serial != null && serial.available() > 0) {
      serialData = serial.readStringUntil('\n'); //read and store in val
    } 

    if (serialData != null && !serialData.equals("")) {
      String[] xy = split(serialData, "\t");
      targetX = float(xy[0]);
      targetY = float(xy[1]);
      x = float(xy[2]);
      y = float(xy[3]);

      xyLabel.setText(int(x) + "/" + int(y));
      xyTarget.setText(int(targetX) + "/" + int(targetY));

      x *= scale;
      y *= scale;
      targetX *= scale;
      targetY *= scale;

      targetX = v.width/2 - targetX;
      targetY = targetY + v.height/2;

      x = v.width/2 - x;
      y = y + v.height/2;

      v.fill(255);
      v.ellipse(x, y, scale*32, scale*32); 

      v.noFill();
      v.stroke(128);
      v.strokeWeight(2);
      v.ellipse(targetX, targetY, scale*40, scale*40); 

      v.stroke(255,0,0);
      v.strokeWeight(1);
      v.line(x, y, targetX, targetY);
      
      float vectorX = 0; 
      float vectorY = 0;
      
      float arrowLen = 10;
      float arrowWidth = 3 * 2; //1.32*2 means 15°
      
      vectorX = -targetX + x;
      vectorY = -targetY + y;
      float len = sqrt(sq(vectorX)+sq(vectorY));
      float dotX = targetX + vectorX / len * arrowLen;
      float dotY = targetY + vectorY / len * arrowLen;
      float perpX = -vectorY;
      float perpY = vectorX;
      float cornerX1 = dotX + perpX / len * arrowWidth/2;      
      float cornerY1 = dotY + perpY / len * arrowWidth/2;
      float cornerX2 = dotX - perpX / len * arrowWidth/2;      
      float cornerY2 = dotY - perpY / len * arrowWidth/2;
      
      v.fill(255,0,0);
      v.triangle(targetX, targetY, cornerX1, cornerY1, cornerX2, cornerY2);
      v.fill(255,0,0);

      v.endDraw();
    }
  }

  void mouseReleased() {
    int clickedX = this.pmouseX()-this.width()/2;
    clickedX = -clickedX;
    int clickedY = this.pmouseY()-this.height()/2;

    clickedX /= scale;
    clickedY /= scale;

    setTarget(clickedX, clickedY);
  }
}
