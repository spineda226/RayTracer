/**
 * OBJFile.hpp
 * 
 * by Brent Williams
 *
 * Notes:
 * - Currently only supports triangles
 */

#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

#include "Triangle.h"
#include "PhongMaterial.h"
//#include "Face.hpp"
#include "AABB.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

//#define MAX_CHARS_PER_LINE 256
//#define MIN_FLOAT -1000000000
//#define MAX_FLOAT 1000000000

//using namespace std;

class OBJFile
{   
   public:
      OBJFile(std::string fileNameVal);
      //const BoundingBox& getBoundingBox();
      //std::vector<Triangle>& getTriangles();
      //unsigned int getNumTriangles();
      void centerMesh();
      AABB *getBoundingBox() const { return boundingBox; }
      std::vector<Triangle *> getTriangles() const { return triangles;}
      std::vector<PhongMaterial> getMaterials() const { return materials; }
      void updateBoundingBox();

   private:
      std::string fileName;
      AABB *boundingBox;
      std::vector<Triangle *> triangles;
      void parse();
      std::vector<PhongMaterial> materials;
};

#endif