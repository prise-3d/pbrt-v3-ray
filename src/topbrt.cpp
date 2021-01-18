#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <filesystem>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "Chemin.hpp"

namespace fs = std::filesystem;

void loadPaths(std::string pathDirName, std::vector<Chemin> &chemins);
bool isPathFile(std::string filename);

int main(int argc, char *argv[]){

  // récupération du nom du dossier contenant les chemins
  if(argc<4){
    std::cout << "syntaxe = " << argv[0] << " --folder <paths directory> --output <output directory>" << std::endl;
    return -1;
  }

  std::string folderName;
  std::string outputName;

  for (int i = 1; i < argc; ++i) {
     if (!strcmp(argv[i], "--folder") || !strcmp(argv[i], "-folder")) {
        folderName = argv[++i];
                                              
     }else if (!strcmp(argv[i], "--output") || !strcmp(argv[i], "-output")) {
        outputName = argv[++i];
     }  
  }

  if (outputName.length() > 0){
    
      try{
       if(fs::create_directory(outputName)) {
          std::cout << "Created output directory\n";
       }
       else {
          std::cerr << "Failed to create a directory\n";
       }                                                       
    }catch(const std::exception& e){
        std::cerr << e.what() << '\n';                    
    }
  }

  std::vector<Chemin> chemins;

  // chargement des chemins
  loadPaths(folderName, chemins);

  // export des chemins au format pbrt
  for(int i=0; i<chemins.size(); i++){
    std::stringstream filename;
   
    // create custom output index for file
    std::string indexStr = std::to_string(i);

    while (indexStr.length() < 5)
        indexStr = "0" + indexStr;

    filename << outputName << "/chemin" << indexStr << ".pbrt";
    chemins[i].toPbrt(filename.str());
  }

  return 1;
}


void loadPaths(std::string pathDirName, std::vector<Chemin> &chemins){

  // ouverture du dossier contenu les fichiers "chemin"
  DIR *dir = opendir (pathDirName.c_str());
  if(!dir){
    std::cout << "erreur ouverture dossier " << pathDirName << std::endl;
    return;
  }

  // récupération des fichiers chemin
  struct dirent* entry;
  
  while ((entry = readdir (dir)) != NULL) {
    std::string filename = pathDirName+"/"+entry->d_name;
    struct stat st;
    lstat(filename.c_str(), &st);
    if(S_ISREG(st.st_mode) && isPathFile(filename)){
       std::cout << "-" << filename << std::endl;
    // lire un chemin
       Chemin path;
       std::ifstream in(filename.c_str());
       if(!in.is_open()){
	 std::cout << "erreur lecture " << filename << std::endl;
       }else{
	 while(path.readPath(in)){
	   //std::cout << path << std::endl;
	   chemins.push_back(path);
	   path.clear();
	 }
	 in.close();
       }
	 
    }
  }// while
  
  closedir(dir);

}

// on vérifie ici juste que l'extension du nom de fichier
// est bien un csv - sans doute prévoir un "magic number" en entête
// pour vérifier indépendamment du nom de fichier
bool isPathFile(std::string filename){
  size_t pos = filename.rfind(".csv"); // recherche de la dernière occurrence
  if(pos!=filename.length()-4) return false;
  return true;
}
