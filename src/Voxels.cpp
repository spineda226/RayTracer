/**
 *  Voxels.cpp
 * 
 * Modified from Brent Williams
 */

#include "Voxels.h"
#include <iostream> // cout, cerr
#include "MortonCode.h"
#include <sstream> // ostringstream
#include "ShadingData.h"

/**
 * Instantiates and initializes the voxel data to be all empty (i.e. 0).
 */
Voxels::Voxels(const unsigned int numLevels, const AABB &boundingBoxVal, const std::vector<Triangle *> *triangles)
 : data(0), 
   boundingBox(boundingBoxVal),
   levels(numLevels),
   size(pow(8, numLevels)), 
   dimension(pow(2, numLevels)),
   voxelWidth(0),
   dataSize(0)
{
   dataSize = calculateDataSize(levels); // calaculate the number of uint64 needed for leaves
   data = new uint64_t[dataSize](); // dynamic memory on the heap | data is a ptr to first element of the block
   
   if (data == NULL) {
       std::string err("\nAllocation failed for data\n");
       std::cerr << err;
       throw std::out_of_range(err);
   }

   boundingBox.square(); // check later -- is this needed? we call this in SVO constructor too
   std::cout << "Voxels constructor boundingBox: " << boundingBox;

   voxelWidth = (boundingBox.getMax().x - boundingBox.getMin().x) / dimension;
   
   std::cout << "Number of 64bit ints allocated: " << dataSize << "\n";
   std::cout << "sizeof(uint64_t) " << sizeof(uint64_t) << "\n";

   voxelTriangleIndexMap = new tbb::concurrent_unordered_map<unsigned int, unsigned int>();
   voxelNormalMap = new tbb::concurrent_unordered_map<unsigned int, ShadingData>();

   // Not using a cache right now
   std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  NO VOXEL CACHE... VOXELIZING NOW  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
   build(triangles);
}

/**
 * Typical destructor releasing the data.
 */
Voxels::~Voxels()
{
   delete [] data;
}

/**
 * Returns the number of 64-bit unsigned ints needed with a set of voxels with levels
 * number of levels.
 * Ex: if there are 3 levels, we need 8 64-bit unsigned ints (512 leaf voxels)
 * Tested: 9-3-2013 
 */
unsigned int Voxels::calculateDataSize(unsigned int levels)
{
   if (levels < 2)
   {
      std::string err("\nInvalid number of levels!\n");
      std::cerr << err;
      throw std::out_of_range(err);
   }
   
   return pow(8, levels-2);
}

/**
 * Builds the volume of voxels from triangles
 * check later: parallelization with mutex
 */
// voxelize each triangle
void Voxels::build(const std::vector<Triangle *> *triangles)
{
   unsigned int stepSize = triangles->size() / 100;
   std::cout << "Triangles: " << triangles->size() << std::endl;
   for (unsigned int i = 0; i < triangles->size(); ++i) {
      voxelizeTriangle(*triangles->at(i), i);
   }
   // Skip for now
   // tbb::atomic<unsigned int> progress = 0;
   // tbb::mutex sm;

   // tbb::parallel_for((unsigned int)0, (unsigned int)triangles->size(), [&](unsigned int i) {

   //    tbb::mutex::scoped_lock lock;
   //    voxelizeTriangle(*triangles->at(i), i);

   //    progress.fetch_and_increment();

   //    if (progress % (stepSize-1) == 0)
   //    {
         
   //       lock.acquire(sm);
   //       float percentDone = (((float) progress) / ( (float)triangles->size() )) * 100.0f;
   //       std::cerr << std::setprecision(3) << "Voxelization: " << percentDone << "%" << std::endl;
   //       lock.release();
   //    }
      
   // });
}

/**
 * Voxelizes the given triangle into the volume 
 */
void Voxels::voxelizeTriangle(const Triangle& triangle, unsigned int i)
{
   vec3 triMins(triangle.getMins()); // gets min coordinates of the triangle
   vec3 triMaxs(triangle.getMaxs()); // gets max coordiantes of the triangle
   
   //Calculate the indexes into the voxel
   unsigned int minX = (triMins.x - boundingBox.getMin().x) / voxelWidth;
   unsigned int maxX = (triMaxs.x - boundingBox.getMin().x + 0) / voxelWidth;
   unsigned int minY = (triMins.y - boundingBox.getMin().y) / voxelWidth;
   unsigned int maxY = (triMaxs.y - boundingBox.getMin().y + 0) / voxelWidth;
   unsigned int minZ = (triMins.z - boundingBox.getMin().z) / voxelWidth;
   unsigned int maxZ = (triMaxs.z - boundingBox.getMin().z + 0) / voxelWidth;
   // std::cout << "boundgin Box:" << boundingBox << std::endl;
   // std::cout << "triMaxZ: " << triMaxs.z << " bbMinz" << boundingBox.getMin().z  << std::endl;
   // std::cout << "MaxZ: " << maxZ << std::endl;
   // std::cout << "Voxel Width: " << voxelWidth << std::endl;
   //Deal with floating point error
   maxX = (maxX >= dimension) ? (dimension-1) : maxX;
   maxY = (maxY >= dimension) ? (dimension-1) : maxY; 
   maxZ = (maxZ >= dimension) ? (dimension-1) : maxZ; 
   //std::cout << "MaxX: " << maxX << " MaxY: " << maxY << " MaxZ: " << maxZ << std::endl;
   
   unsigned int x, y, z;
   vec3 deltaP(voxelWidth, voxelWidth, voxelWidth); //The (maxs-mins) of the 
                                                    //voxel's bounding box
   //std::cout << " boundingBox: " << boundingBox;
   for (x = minX; x <= maxX; x++)
   {
      for (y = minY; y <= maxY; y++)
      {
         for (z = minZ; z <= maxZ; z++)
         {
            vec3 p(boundingBox.getMin().x + (x*voxelWidth),  //The mins of the 
             boundingBox.getMin().y + (y*voxelWidth),        //voxel's bounding box
             boundingBox.getMin().z + (z*voxelWidth) );
             
             // check later; if a voxel is already set do I still perform calculation?
            //if (triangle.triangleAABBIntersect(p, deltaP))
            if (triangle.myAABBTest(p, deltaP))
            {
               //std::cout << "Intersection! " << i << std::endl;
               unsigned int mortonIndex = mortonCode(x, y, z, levels);
               //glm::vec3 v0(triangle.v0.x,triangle.v0.y,triangle.v0.z);
               //glm::vec3 v1(triangle.v1.x,triangle.v1.y,triangle.v1.z);
               //glm::vec3 v2(triangle.v2.x,triangle.v2.y,triangle.v2.z);

               // Calculate the normal of the triangle/plane
               // glm::vec3 normal = glm::normalize( glm::cross(v1-v0, v2-v0) );
               // cout << "Voxelization: (" << x << ", " << y << ", " << z << ") => mortonIndex: " << mortonIndex << endl;
               // cout << "\tTriangle (" << i << ") " << endl;
               // cout << "\tTriangle Normal: <" << normal.x << ", " << normal.y << ", " << normal.z << ">" << endl; 
               // cout << endl;
               //std::cout << "MI: " << mortonIndex << "\n";

               voxelTriangleIndexMap->insert( std::make_pair<unsigned int, unsigned int>( (unsigned int)mortonIndex, (unsigned int)i ) );
               vec3 normal = glm::normalize( glm::cross(triangle.getP2() - triangle.getP1(), triangle.getP3()-triangle.getP1()));
               unsigned int matIdx = triangle.getMatIdx();
               voxelNormalMap->insert(std::make_pair((unsigned int)mortonIndex, ShadingData(normal, matIdx)));

               set(x,y,z);
            }
         }
      }
   }

   /* Morton Code Testing */
   // minX = 0;
   // maxX = dimension-1;
   // minY = 0;
   // maxY = dimension-1;
   // minZ = 0;
   // maxZ = dimension-1;
   // for (x = minX; x <= maxX; x++)
   // {
   //    for (y = minY; y <= maxY; y++)
   //    {
   //       for (z = minZ; z <= maxZ; z++)
   //       {
   //          unsigned int mortonIndex = mortonCode(x, y, z, levels);
   //          std::cout << x << " " << y << " " << z << ": " << mortonIndex << "\n";
   //       }
   //    }
   // }
}

/**
 * Return the set of 64 voxels (i.e. an 64 bit int or int64_t) that are the voxels 
 * at the index i.
 *
 * Tested: 9-3-2013 
 */
uint64_t& Voxels::operator[](unsigned int i)
{
   if (i >= dataSize)
   {
      std::string err = "\nIndex out of range in Voxels::operator[] with an index of ";
      std::ostringstream convert;
      convert << i;
      err += convert.str();
      std::cerr << err;
      throw std::out_of_range(err);
   }
   return data[i];
}

/**
 * Sets the voxel at the given x, y and z values as filled 
 *
 * Tested: 9-3-2013 
 * check later: mutex deprecated?
 */
void Voxels::set(unsigned int x, unsigned int y, unsigned int z)
{
   //If each individual voxel had an index, the voxeNumber is that index
   unsigned int voxelNumber = mortonCode(x,y,z,levels);
   //std::cout << "Voxelization Voxel Number: " << voxelNumber << "\n";
   //unsigned int voxelNumber = x + dimension*y + dimension*dimension*z;
   
   //dataIndex is the index into the uint64 array of the current voxel
   unsigned int dataIndex = voxelNumber / 64;
   
   //bitIndex is the current voxel (represented by a bit) to set
   unsigned int bitIndex =  voxelNumber % 64;
   
   //The mask used to set the voxel
   uint64_t toOr = (1L << bitIndex);
   
   tbb::mutex::scoped_lock lock;
   lock.acquire(sMutex);
   //#pragma omp atomic
   data[dataIndex] |= toOr; // sets the bitIndex bit 
   lock.release();
}
