char AcXStr[7], AcYStr[7], AcZStr[7];
char GyXStr[7], GyYStr[7], GyZStr[7];

float fact;

float xx, xy, xz;
float yx, yy, yz;
float zx, zy, zz;
int Xan, Yan;
int Xoff, Yoff, Zoff;

struct Point3d
{
  int x;
  int y;
  int z;
};

struct Point2d
{
  int x;
  int y;
};

int LinestoRender; 
int OldLinestoRender; 

struct Line3d
{
  Point3d p0;
  Point3d p1;
};

struct Line2d
{
  Point2d p0;
  Point2d p1;
};

Line3d Lines[12];  
Line2d Render[12];
Line2d ORender[12];
