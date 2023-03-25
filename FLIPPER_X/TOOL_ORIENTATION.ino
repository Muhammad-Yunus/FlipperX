void display_gyroscope(){
  mpu_o.getMotion6(&AcY, &AcX, &AcZ, &GyY, &GyX, &GyZ);
  calculateCube();
  drawScreen(); 
  display_navigation(false, true, false);
  display.display(); 
}

void display_gyroscope_init(){
  display.clearDisplay();
  initLine();
  initDrawValue(); 
}

void initDrawValue(){
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Xa:");
  display.setCursor(0, 10);
  display.print("Ya:"); 
  display.setCursor(0, 20);
  display.print("Za:"); 
  display.setCursor(0, 30);
  display.print("Xg:"); 
  display.setCursor(0, 40);
  display.print("Yg:"); 
  display.setCursor(0, 50);
  display.print("Zg:"); 
}

void mpuOffsetConfig(int XAOffset, int YAOffset, int ZAOffset, int XGOffset, int YGOffset, int ZGOffset) {
  Serial.println(mpu_o.testConnection() ? "mpu_o connection successful" : "MPU6050 connection failed");

  mpu_o.setXAccelOffset(XAOffset); 
  mpu_o.setYAccelOffset(YAOffset); 
  mpu_o.setZAccelOffset(ZAOffset); 
  mpu_o.setXGyroOffset (XGOffset); 
  mpu_o.setYGyroOffset (YGOffset); 
  mpu_o.setZGyroOffset (ZGOffset); 
}

void initLine() {
  fact = 180 / 3.14159265358979323846264338327950; 

  Xoff = 90; 
  Yoff = 32; 
  Zoff = 750; 

  // line segments to draw a cube. basically p0 to p1. p1 to p2. p2 to p3 so on.

  // Front Face.
  
  Lines[0].p0.x = -50;
  Lines[0].p0.y = -50;
  Lines[0].p0.z = 50;
  Lines[0].p1.x = 50;
  Lines[0].p1.y = -50;
  Lines[0].p1.z = 50;

  Lines[1].p0.x = 50;
  Lines[1].p0.y = -50;
  Lines[1].p0.z = 50;
  Lines[1].p1.x = 50;
  Lines[1].p1.y = 50;
  Lines[1].p1.z = 50;

  Lines[2].p0.x = 50;
  Lines[2].p0.y = 50;
  Lines[2].p0.z = 50;
  Lines[2].p1.x = -50;
  Lines[2].p1.y = 50;
  Lines[2].p1.z = 50;

  Lines[3].p0.x = -50;
  Lines[3].p0.y = 50;
  Lines[3].p0.z = 50;
  Lines[3].p1.x = -50;
  Lines[3].p1.y = -50;
  Lines[3].p1.z = 50;


  //back face.

  Lines[4].p0.x = -50;
  Lines[4].p0.y = -50;
  Lines[4].p0.z = -50;
  Lines[4].p1.x = 50;
  Lines[4].p1.y = -50;
  Lines[4].p1.z = -50;

  Lines[5].p0.x = 50;
  Lines[5].p0.y = -50;
  Lines[5].p0.z = -50;
  Lines[5].p1.x = 50;
  Lines[5].p1.y = 50;
  Lines[5].p1.z = -50;

  Lines[6].p0.x = 50;
  Lines[6].p0.y = 50;
  Lines[6].p0.z = -50;
  Lines[6].p1.x = -50;
  Lines[6].p1.y = 50;
  Lines[6].p1.z = -50;

  Lines[7].p0.x = -50;
  Lines[7].p0.y = 50;
  Lines[7].p0.z = -50;
  Lines[7].p1.x = -50;
  Lines[7].p1.y = -50;
  Lines[7].p1.z = -50;

  // now the 4 edge lines.

  Lines[8].p0.x = -50;
  Lines[8].p0.y = -50;
  Lines[8].p0.z = 50;
  Lines[8].p1.x = -50;
  Lines[8].p1.y = -50;
  Lines[8].p1.z = -50;

  Lines[9].p0.x = 50;
  Lines[9].p0.y = -50;
  Lines[9].p0.z = 50;
  Lines[9].p1.x = 50;
  Lines[9].p1.y = -50;
  Lines[9].p1.z = -50;

  Lines[10].p0.x = -50;
  Lines[10].p0.y = 50;
  Lines[10].p0.z = 50;
  Lines[10].p1.x = -50;
  Lines[10].p1.y = 50;
  Lines[10].p1.z = -50;

  Lines[11].p0.x = 50;
  Lines[11].p0.y = 50;
  Lines[11].p0.z = 50;
  Lines[11].p1.x = 50;
  Lines[11].p1.y = 50;
  Lines[11].p1.z = -50;

  LinestoRender = 12;
  OldLinestoRender = LinestoRender;
}

void SetVars(void)
{
  float Xan2, Yan2, Zan2;
  float s1, s2, s3, c1, c2, c3;

  // convert degrees to radians.
  Xan2 = Xan / fact; 
  Yan2 = Yan / fact;
  
  s1 = sin(Yan2);
  s2 = sin(Xan2);

  c1 = cos(Yan2);
  c2 = cos(Xan2);

  xx = c1;
  xy = 0;
  xz = -s1;

  yx = (s1 * s2);
  yy = c2;
  yz = (c1 * s2);

  zx = (s1 * c2);
  zy = -s2;
  zz = (c1 * c2);
}

void ProcessLine(struct Line2d *ret, struct Line3d vec)
{
  float zvt1;
  int xv1, yv1, zv1;

  float zvt2;
  int xv2, yv2, zv2;

  int rx1, ry1;
  int rx2, ry2;

  int x1;
  int y1;
  int z1;

  int x2;
  int y2;
  int z2;

  int Ok;

  x1 = vec.p0.x;
  y1 = vec.p0.y;
  z1 = vec.p0.z;

  x2 = vec.p1.x;
  y2 = vec.p1.y;
  z2 = vec.p1.z;

  Ok = 0; // defaults to not OK

  xv1 = (x1 * xx) + (y1 * xy) + (z1 * xz);
  yv1 = (x1 * yx) + (y1 * yy) + (z1 * yz);
  zv1 = (x1 * zx) + (y1 * zy) + (z1 * zz);

  zvt1 = zv1 - Zoff;

  if ( zvt1 < -5) {
    rx1 = 256 * (xv1 / zvt1) + Xoff;
    ry1 = 256 * (yv1 / zvt1) + Yoff;
    Ok = 1; // ok we are alright for point 1.
  }

  xv2 = (x2 * xx) + (y2 * xy) + (z2 * xz);
  yv2 = (x2 * yx) + (y2 * yy) + (z2 * yz);
  zv2 = (x2 * zx) + (y2 * zy) + (z2 * zz);

  zvt2 = zv2 - Zoff;

  if ( zvt2 < -5) {
    rx2 = 256 * (xv2 / zvt2) + Xoff;
    ry2 = 256 * (yv2 / zvt2) + Yoff;
  } else
  {
    Ok = 0;
  }

  if (Ok == 1) {
    ret->p0.x = rx1;
    ret->p0.y = ry1;

    ret->p1.x = rx2;
    ret->p1.y = ry2;
  }
}

void calculateCube() {
  //For cube rotation
  int xOut = 0;
  int yOut = 0;

  xOut = map(AcX, -17000, 17000, -50, 50);
  yOut = map(AcY, -17000, 17000, -50, 50);

  Xan += xOut;
  Yan += yOut;

  // prevents overflow.
  Yan = Yan % 360;
  Xan = Xan % 360; 

  SetVars(); 

  for (int i = 0; i < LinestoRender ; i++)
  {
    ORender[i] = Render[i]; 
    ProcessLine(&Render[i], Lines[i]); 
  }
}

void drawScreen( void)
{  
  display.setTextSize(1);
  for (int i = 0; i < OldLinestoRender; i++ )
  {
    display.drawLine(ORender[i].p0.x, ORender[i].p0.y, ORender[i].p1.x, ORender[i].p1.y, BLACK); // erase the old lines.
  }

  display.setTextColor(BLACK);
  display.setCursor(19, 0);
  display.print(AcXStr); 
  display.setCursor(19, 10);
  display.print(AcYStr); 
  display.setCursor(19, 20);
  display.print(AcZStr); 
  display.setCursor(19, 30);
  display.print(GyXStr); 
  display.setCursor(19, 40);
  display.print(GyYStr); 
  display.setCursor(19, 50);
  display.print(GyZStr); 
  
  for (int i = 0; i < LinestoRender; i++ )
  {
    display.drawLine(Render[i].p0.x, Render[i].p0.y, Render[i].p1.x, Render[i].p1.y, WHITE);
  }
  OldLinestoRender = LinestoRender;
  
  sprintf (AcXStr, "%6d", AcX);
  sprintf (AcYStr, "%6d", AcY);
  sprintf (AcZStr, "%6d", AcZ);
  sprintf (GyXStr, "%6d", GyX);
  sprintf (GyYStr, "%6d", GyY);
  sprintf (GyZStr, "%6d", GyZ);

  display.setTextColor(WHITE);
  display.setCursor(19, 0);
  display.print(AcXStr); 
  display.setCursor(19, 10);
  display.print(AcYStr); 
  display.setCursor(19, 20);
  display.print(AcZStr); 
  display.setCursor(19, 30);
  display.print(GyXStr); 
  display.setCursor(19, 40);
  display.print(GyYStr); 
  display.setCursor(19, 50);
  display.print(GyZStr); 
}
