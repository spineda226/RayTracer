/**
 * OBJFile.cpp
 * 
 * by Brent Williams
 *
 * Notes:
 * - Currently only supports triangles
 */

#include "OBJFile.h"

OBJFile::OBJFile(std::string fileNameVal)
 : fileName(fileNameVal),
   triangles()
{
   boundingBox = new AABB();
   parse();
}

void OBJFile::parse()
{
   Assimp::Importer importer;
   // And have it read the given file with some example postprocessing
   // Usually - if speed is not the most important aspect for you - you'll 
   // propably to request more postprocessing than we do in this example.
   
   std::cerr << "Reading in file...\n";
   const aiScene* scene = importer.ReadFile( fileName, 
        aiProcess_CalcTangentSpace       | 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);
  
   // If the import failed, report it
   if( !scene)
   {
      std::cerr << "ERROR: Failed to import mesh!!!\n";
   }

   for (int i = 0; i < scene->mNumMeshes; i++)
   {
      const struct aiMesh* mesh = scene->mMeshes[i];
      unsigned int materialIndex = mesh->mMaterialIndex;
      std::cout << "materialIndex = " << materialIndex << std::endl;
      std::cout << "num triangles in mesh: " << mesh->mNumFaces << "\n"; 
      for (int f = 0; f < mesh->mNumFaces; f++) 
      {
         const struct aiFace* face = &mesh->mFaces[f];
         if (face->mNumIndices != 3)
         {
            std::cerr << "ERROR: Found non-triangle geometry!\n";
            return;
         }
         int vertexIndex0 = face->mIndices[0];
         int vertexIndex1 = face->mIndices[1];
         int vertexIndex2 = face->mIndices[2];

         glm::vec3 v0 = glm::vec3(mesh->mVertices[vertexIndex0].x, mesh->mVertices[vertexIndex0].y, mesh->mVertices[vertexIndex0].z);
         glm::vec3 v1 = glm::vec3(mesh->mVertices[vertexIndex1].x, mesh->mVertices[vertexIndex1].y, mesh->mVertices[vertexIndex1].z);
         glm::vec3 v2 = glm::vec3(mesh->mVertices[vertexIndex2].x, mesh->mVertices[vertexIndex2].y, mesh->mVertices[vertexIndex2].z);

         Triangle *triangle = new Triangle(v0,v1,v2,materialIndex);
         float low = 0.1;
         float high = 1;
         float r = (((double) rand())/(double)RAND_MAX)*(high-low)+low;
         float g = (((double) rand())/(double)RAND_MAX)*(high-low)+low;
         float b = (((double) rand())/(double)RAND_MAX)*(high-low)+low;
         triangle->setColor(vec3(r, g, b));
         triangles.push_back(triangle);
      }
   }

   for (int i = 0; i < scene->mNumMaterials; i++)
   {
      const struct aiMaterial* mtl = scene->mMaterials[i];
      int ret1, ret2;
      unsigned int max;
      float shininess, strength;

      aiColor4D kdAi;
      aiColor4D ksAi;
      aiColor4D kaAi;

      glm::vec3 ka(0.1f,0.1f,0.1f);
      glm::vec3 kd(0.5f,0.5f,0.5f);
      glm::vec3 ks(0.5f,0.5f,0.5f);
      float ns = 1.0f;

      if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &kaAi))
      {
         ka = glm::vec3(kaAi.r, kaAi.g, kaAi.b);
      }

      if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &kdAi))
      {
         kd = glm::vec3(kdAi.r, kdAi.g, kdAi.b);
      }

      if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &ksAi))
      {
         ks = glm::vec3(ksAi.r, ksAi.g, ksAi.b);
      }

      max = 1;
      ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
      max = 1;
      ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);

      if(ret1 == AI_SUCCESS)
      {
         ns = shininess / 4.0f; 
      }

      PhongMaterial material(ka,kd,ks,ns); //commented out by Steve
      materials.push_back(material); //commented out by Steve

      std::cout << std::endl;
      std::cout << "material " << i << ":\n";
      std::cout << "\tambient: <" << ka.x << ", " << ka.y << ", " << ka.z << ">\n";
      std::cout << "\tdiffuse: <" << kd.x << ", " << kd.y << ", " << kd.z << ">\n";
      std::cout << "\tspecular: <" << ks.x << ", " << ks.y << ", " << ks.z << ">\n";
      std::cout << "\tshininess: " << ns << std::endl;
      std::cout << std::endl;
   }

   // cout << "Num triangles: " << triangles.size() << endl << endl;
   std::cerr << "Finished reading in file\n\n";
   updateBoundingBox();
}

// /**
//  * Returns the bounding box for the geometry in the obj file. 
//  *
//  * Note: Not guaranteed to be square
//  */
// const BoundingBox& OBJFile::getBoundingBox()
// {
//    return boundingBox;
// }

// /**
//  * Returns a vector of all the triangles in the obj file.
//  */
// std::vector<Triangle>& OBJFile::getTriangles()
// {
//    return triangles;
// }

// /**
//  * Returns the number of triangles it has.
//  *
//  */
// unsigned int OBJFile::getNumTriangles()
// {
//    return triangles.size();
// }

void OBJFile::centerMesh()
{
   // Center the mesh around (0, 0, 0)
   glm::vec3 translate = boundingBox->getCenter();

   for (unsigned long i = 0; i < triangles.size(); i++)
   {
      Triangle *triangle = triangles.at(i);
      triangle->updateP1(-translate);
      triangle->updateP2(-translate);
      triangle->updateP3(-translate);
   }
   std::cout << boundingBox->getMin().x << " min\n";
   boundingBox->updateMin(-translate);
   boundingBox->updateMax(-translate);
   //updateBoundingBox();
}

void OBJFile::updateBoundingBox()
{
   std::cerr << "triangles.size() = " << triangles.size() << std::endl;
   glm::vec3 mins = triangles[0]->getP1();
   glm::vec3 maxs = triangles[0]->getP2();
   for (unsigned long i = 0; i < triangles.size(); i++)
   {
      glm::vec3 v = triangles[i]->getP1();
      if (v.x < mins.x)
         mins.x = v.x;
      if (v.y < mins.y)
         mins.y = v.y;
      if (v.z < mins.z)
         mins.z = v.z;
            
      if (v.x > maxs.x)
         maxs.x = v.x;
      if (v.y > maxs.y)
         maxs.y = v.y;
      if (v.z > maxs.z)
         maxs.z = v.z;

      v = triangles[i]->getP2();
      if (v.x < mins.x)
         mins.x = v.x;
      if (v.y < mins.y)
         mins.y = v.y;
      if (v.z < mins.z)
         mins.z = v.z;
            
      if (v.x > maxs.x)
         maxs.x = v.x;
      if (v.y > maxs.y)
         maxs.y = v.y;
      if (v.z > maxs.z)
         maxs.z = v.z;

      v = triangles[i]->getP3();
      if (v.x < mins.x)
         mins.x = v.x;
      if (v.y < mins.y)
         mins.y = v.y;
      if (v.z < mins.z)
         mins.z = v.z;
            
      if (v.x > maxs.x)
         maxs.x = v.x;
      if (v.y > maxs.y)
         maxs.y = v.y;
      if (v.z > maxs.z)
         maxs.z = v.z;
   }
   boundingBox->setMin(mins);
   boundingBox->setMax(maxs);
}