#include "Chemin.hpp"
#include <cmath>

bool Chemin::readPath(std::ifstream &in){
  char k; // utiliser pour récupérer le caractère séparateur

  // récupération éventuelle des coordonnées du pixel
  in >> x >> k >> y >> k;
  if(in.eof()) return false;

  std::cout << "==============================================================================" << std::endl;
  std::cout << "Origin(" << x << ", " << y << ")" << std::endl;

  // récupération des noeuds du chemin
  std::string ligne;
  getline(in, ligne);
  std::istringstream iss(ligne);
  
  Point s;
  Color currentL;
  unsigned counter = 0;
  iss >> s.x;// on tente un premier sommet
  while(iss){// il y a un sommet à lire
    
    // récupération des coordonées
    iss >> k >> s.y >> k >> s.z >> k;
    sommets.push_back(s);
    std::cout << "Point(" << s.x << ", " << s.y << ", " << s.z << ")";
    
    // récupération des luminances
    // La première luminance correspond à la somme, on la stocke donc en information globale
    if (counter == 0) {
       iss >> l.r >> k >> l.g >> k >> l.b >> k;  
       std::cout << " => Sum of L(" << l.r << ", " << l.g << ", " << l.b << ")" << std::endl;
    }
    else{
       iss >> currentL.r >> k >> currentL.g >> k >> currentL.b >> k;  
       luminances.push_back(currentL);
       std::cout << " => L(" << currentL.r << ", " << currentL.g << ", " << currentL.b << ")" << std::endl;
    }

    iss >> s.x;// on tente le sommet suivant
    counter++;
  }


  return true;
}


void Chemin::toPbrt(std::string filename){
  std::cout << "sortie dans " << filename << std::endl;
  std::ofstream out(filename);
  if(!out.is_open()){
    std::cerr << "erreur d'ouverture du fichier " << filename << std::endl;
    return;
  }

  // export du materiau des cylindres
  out << "MakeNamedMaterial \"Rayon\"" << std::endl;
  out << "\t\"string type\" [\"uber\"]" << std::endl;
  out << "\t\"color Kd\" [0.43298 0.043298 0.043298]" << std::endl;
  out << "\t\"color Ks\" [0.47366 0.047366 0.047366]" << std::endl;
  out << "\t\"color Kr\" [0.000000 0.000000 0.000000]" << std::endl;
  out << std::endl;

  out << "MakeNamedMaterial \"HighRayon\"" << std::endl;
  out << "\t\"string type\" [\"uber\"]" << std::endl;
  out << "\t\"color Kd\" [0.20 0.043298 0.743298]" << std::endl;
  out << "\t\"color Ks\" [0.20 0.047366 0.77366]" << std::endl;
  out << "\t\"color Kr\" [0.000000 0.000000 0.000000]" << std::endl;
  out << std::endl;

  // insert camera overview as sphere
  out << "AttributeBegin" << std::endl;
  out << "\tNamedMaterial \"Rayon\"" << std::endl;
  out << "\tTranslate " << sommets[0].x << " " << sommets[0].y << " " << sommets[0].z << std::endl;
  out << "\tShape \"sphere\" \"float radius\" 0.5" << std::endl;
  out << "AttributeEnd" << std::endl;
  out << std::endl;

  // export de chaque élément du chemin sous forme d'un cylindre
  // passage des luminances acquises au rebond
  for(int i=0; i<sommets.size()-1; i++)
    exportCylindre(sommets[i], sommets[i+1], luminances[i], out);

  out.close();
}



void Chemin::exportCylindre(const Point &c1, const Point &c2, const Color &l, std::ofstream &out){
  // on translate le cylindre à l'origine
  Point t = c1;
  Point o1 = c1-t;
  Point o2 = c2-t;
  // on stocke la hauteur du cylindre
  Vecteur u(o1, o2);
  float hauteur = u.norme();

  // calcul du vecteur perpendiculaire à Oz et u
  u.normalise();
  Vecteur oz(0,0,1);
  Vecteur v;
  v = u.cross(oz);
  
  // calcul de l'angle de rotation de u vers oz autour de v
  float angle = std::asin(v.norme()); // valeur en radian
  angle = angle * 180.0 / M_PI; // valeur en degré

  // mise à jour de l'angle de rotation pbrt
  if(u.z <0) // axe z en sens inverse de l'axe du cylindre
    angle = -180.0 + angle;
  else
    angle = -angle;
      
  // sortie du cylindre pbrt
  out << "AttributeBegin" << std::endl;

  // Ray cylinder color depending on contribution
  if (l.r > limit || l.g > limit || l.b > limit)
     out << "\tNamedMaterial \"HighRayon\"" << std::endl;
  else
     out << "\tNamedMaterial \"Rayon\"" << std::endl;
  out << "\tTranslate " << t.x << " " << t.y << " " << t.z << std::endl;
  out << "\tRotate " << angle << " " << v.x << " " << v.y << " " << v.z << std::endl;
  out << "\tShape \"cylinder\" \"float radius\" [0.01] \"float zmin\"  [0.0] ";
  out << "\"float zmax\" [" << hauteur << "]" << std::endl;
  out << "AttributeEnd" << std::endl << std::endl;
  
  // out << c1.x << " " << c1.y << " " << c1.z << " - ";
  // out << c2.x << " " << c2.y << " " << c2.z << std::endl;
}

std::ostream& operator<<(std::ostream& out, const Chemin& path){
  out << path.x << "-" << path.y << " (";
  out << path.l.r << "-" << path.l.g <<  "-" << path.l.b << ") ";
  for(int i=0; i<path.sommets.size(); i++){
    out << "[" << path.sommets[i].x;
    out << "," << path.sommets[i].y;
    out << "," << path.sommets[i].z;
    out << "] ";
  }

  return out;
}
