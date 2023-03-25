void display_compas(){
  display.clearDisplay();
  
  sensors_event_t event; 
  mag.getEvent(&event);
  
  int16_t x,y,z; 

  x = event.magnetic.x;
  z = event.magnetic.z;
  y = event.magnetic.y;

  Draw_Compass_Rose(); 
  float heading = atan2(y, x); 
  float declination = -0.041;
  heading = heading + declination;
  if(heading < 0)    heading += 2*PI;  
  if(heading > 2*PI) heading -= 2*PI;  
  angle = int(heading * 180/M_PI);

  dx = (0.7*radius * cos((angle-90)*3.14/180)) + centreX;  
  dy = (0.7*radius * sin((angle-90)*3.14/180)) + centreY;  
  arrow(last_dx,last_dy, centreX, centreY, 2,2,BLACK);     
  arrow(dx,dy, centreX, centreY, 2, 2,WHITE);              
  last_dx = dx; 
  last_dy = dy;

  display.setCursor(0,38);
  display.fillRect(0,38,25,48,BLACK);
  display.print(angle);
  display.print(char(247));

  display_navigation(false, true, false);
  
  display.display();
}

void display_item(int x, int y, String token, int txt_colour, int txt_size) {
  display.setCursor(x, y);
  display.setTextColor(txt_colour);
  display.setTextSize(txt_size);
  display.print(token);
  display.setTextSize(1);
}

void arrow(int x2, int y2, int x1, int y1, int alength, int awidth, int colour) {
  float distance;
  int dx, dy, x2o,y2o,x3,y3,x4,y4,k;
  distance = sqrt(pow((x1 - x2),2) + pow((y1 - y2), 2));
  dx = x2 + (x1 - x2) * alength / distance;
  dy = y2 + (y1 - y2) * alength / distance;
  k = awidth / alength;
  x2o = x2 - dx;
  y2o = dy - y2;
  x3 = y2o * k + dx;
  y3 = x2o * k + dy;
  x4 = dx - y2o * k;
  y4 = dy - x2o * k;
  display.drawLine(x1, y1, x2, y2, colour);
  display.drawLine(x1, y1, dx, dy, colour);
  display.drawLine(x3, y3, x4, y4, colour);
  display.drawLine(x3, y3, x2, y2, colour);
  display.drawLine(x2, y2, x4, y4, colour);
} 

void Draw_Compass_Rose() {
  int dxo, dyo, dxi, dyi;
  display.drawCircle(centreX,centreY,radius,WHITE);
  for (float i = 0; i <360; i = i + 22.5) {
    dxo = radius * cos(i*3.14/180);
    dyo = radius * sin(i*3.14/180);
    dxi = dxo * 0.95;
    dyi = dyo * 0.95;
    display.drawLine(dxi+centreX,dyi+centreY,dxo+centreX,dyo+centreY,WHITE);   
  }
  display_item((centreX-2),(centreY-24),"N",WHITE,1);
  display_item((centreX-2),(centreY+17),"S",WHITE,1);
  display_item((centreX+19),(centreY-3),"E",WHITE,1);
  display_item((centreX-23),(centreY-3),"W",WHITE,1);
}
