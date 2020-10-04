/**
 *  Voxels.cpp
 * 
 * Modified from Brent Williams
 */

#include "Voxels.h"
#include <iostream> // cout, cerr

/**
 * Instantiates and initializes the voxel data to be all empty (i.e. 0).
 */
Voxels::Voxels(const unsigned int numLevels, AABB &boundingBox, const std::vector<Triangle *> *triangles)
 : data(0), 
   boundingBox(boundingBox),
   levels(numLevels),
   size(pow(8, numLevels)), 
   dimension(pow(2, numLevels)),
   voxelWidth(0),
   dataSize(0)
{
   dataSize = calculateDataSize(levels); // calaculate the number of uint64 needed for leaves
   data = new uint64_t[dataSize](); // dynamic memory on the heap
   
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
void Voxels::build(const std::vector<Triangle *> *triangles)
{
   unsigned int stepSize = triangles->size() / 100;
   for (unsigned int i = 0; i < triangles->size(); ++i) {
      voxelizeTriangle(*triangles->at(i), i);
   }
   // Skip for now
   // tbb::atomic<unsigned int> progress = 0;
   // tbb::mutex sm;

   // tbb::parallel_for((unsigned int)0, (unsigned int)triangles.size(), [&](unsigned int i) {

   //    tbb::mutex::scoped_lock lock;
   //    voxelizeTriangle(triangles[i], i);

   //    progress.fetch_and_increment();

   //    if (progress % (stepSize-1) == 0)
   //    {
         
   //       lock.acquire(sm);
   //       float percentDone = (((float) progress) / ( (float)triangles.size() )) * 100.0f;
   //       cerr << setprecision(3) << "Voxelization: " << percentDone << "%" << endl;
   //       lock.release();
   //    }
      
   // });
}

/**
 * Voxelizes the given triangle into the volume 
 */
void Voxels::voxelizeTriangle(const Triangle& triangle, unsigned int i)
{
   vec3 triMins(triangle.getMins());
   vec3 triMaxs(triangle.getMaxs());
   
   //Calculate the indexes into the voxel
   unsigned int minX = (triMins.x - boundingBox.getMin().x) / voxelWidth;
   unsigned int maxX = (triMaxs.x - boundingBox.getMin().x + 0.5) / voxelWidth;
   unsigned int minY = (triMins.y - boundingBox.getMin().y) / voxelWidth;
   unsigned int maxY = (triMaxs.y - boundingBox.getMin().y + 0.5) / voxelWidth;
   unsigned int minZ = (triMins.z - boundingBox.getMin().z) / voxelWidth;
   unsigned int maxZ = (triMaxs.z - boundingBox.getMin().z + 0.5) / voxelWidth;
   
   //Deal with floating point error
   maxX = (maxX >= dimension) ? (dimension-1) : maxX;
   maxY = (maxY >= dimension) ? (dimension-1) : maxY; 
   maxZ = (maxZ >= dimension) ? (dimension-1) : maxZ;
   
   unsigned int x, y, z;
   vec3 deltaP(voxelWidth, voxelWidth, voxelWidth); //The (maxs-mins) of the 
                                                    //voxel's bounding box
   
   // for (x = minX; x <= maxX; x++)
   // {
   //    for (y = minY; y <= maxY; y++)
   //    {
   //       for (z = minZ; z <= maxZ; z++)
   //       {
   //          Vec3 p(boundingBox.mins.x + (x*voxelWidth),  //The mins of the 
   //           boundingBox.mins.y + (y*voxelWidth),        //voxel's bounding box
   //           boundingBox.mins.z + (z*voxelWidth) );
             
   //          if (triangleAABBIntersect(triangle, p, deltaP))
   //          {
   //             unsigned int mortonIndex = mortonCode(x, y, z, levels);
   //             glm::vec3 v0(triangle.v0.x,triangle.v0.y,triangle.v0.z);
   //             glm::vec3 v1(triangle.v1.x,triangle.v1.y,triangle.v1.z);
   //             glm::vec3 v2(triangle.v2.x,triangle.v2.y,triangle.v2.z);

   //             // Calculate the normal of the triangle/plane
   //             // glm::vec3 normal = glm::normalize( glm::cross(v1-v0, v2-v0) );
   //             // cout << "Voxelization: (" << x << ", " << y << ", " << z << ") => mortonIndex: " << mortonIndex << endl;
   //             // cout << "\tTriangle (" << i << ") " << endl;
   //             // cout << "\tTriangle Normal: <" << normal.x << ", " << normal.y << ", " << normal.z << ">" << endl; 
   //             // cout << endl;

   //             voxelTriangleIndexMap->insert( std::make_pair<unsigned int,unsigned int>( (unsigned int)mortonIndex, (unsigned int)i ) );

   //             set(x,y,z);
   //          }
   //       }
   //    }
   // }     
}