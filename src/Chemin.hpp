#ifndef _CHEMIN_HPP
#define _CHEMIN_HPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

struct Color {
    float r, g, b;
    Color() { r=g=b=0; }
    Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

struct Point {
  float x, y, z;
  Point(){ x=y=z=0; }
  Point(float x, float y, float z) : x(x), y(y), z(z){}
  void operator-=(const Point &t){
    x-= t.x; y-=t.y; z -= t.z;
  }
  Point operator-(const Point &p) const{
    return Point(x-p.x, y-p.y, z-p.z);
  }
};

struct Vecteur {
  float x, y, z;
  Vecteur(){ x=y=z=0; }
  Vecteur(float x, float y, float z) : x(x), y(y), z(z){}
  Vecteur(const Point &a, const Point &b){
    x = b.x-a.x; y = b.y-a.y; z = b.z - a.z;
  }
  float norme(){
    return std::sqrt(x*x+y*y+z*z);
  }
  void normalise(){
    float lg = norme();
    x /= lg; y /= lg; z /= lg;
  }
  float dot(const Vecteur &v){
    return (x*v.x+y*v.y+z*v.z);
  }
  Vecteur cross(const Vecteur &v){
    Vecteur r;
    r.x = y*v.z-z*v.y;
    r.y = z*v.x-x*v.z;
    r.z = x*v.y-y*v.x;
    return r;
  }
};
class Chemin {
public:
  int x, y; // pixel initial du chemin
  Color l; // final luminance obtained
  std::vector<Point> sommets; // sommets du chemin
  std::vector<Color> luminances; // luminances acquises

  Chemin(){};

  bool readPath(std::ifstream &in);
  
  void toPbrt(std::string filename);

  void clear(){ sommets.clear(); }

  
  friend std::ostream& operator<<(std::ostream& out, const Chemin& path);

private:

  // set limit to change display of cylinder material
  float limit = 10000;

  void exportCylindre(const Point &c1, const Point &c2, const Color &l, std::ofstream &out);


};

#endif
