/* Class for Sparse Voxel Octrees
 *  Modified from Brent Williams
 */

#ifndef _SVO_H_
#define _SVO_H_

#include "Triangle.h"
#include <glm/glm.hpp>

#include "AABB.h"
//#include "SVONode.hpp"

//#include <vector>
//#include <stdint.h>
//#include <string>
//#include <unordered_map>
//#include "tbb/concurrent_unordered_map.h"

#include <chrono>

#define SET_8_BITS 255

using namespace std;
using namespace glm;

class SVO
{
  public:
    SVO(const unsigned int numLevels, AABB& boundingBox, const std::vector<Triangle *> *triangles);
		~SVO() {};

    void build(const std::vector<Triangle *> *triangles);

	private:
    unsigned int numLevels; // depth (root is level 0), needs to be at least 3 levels
		AABB boundingBox;
		unsigned long size; // Total number of voxels if the SVO was full: 8**numLevels
    unsigned int dimension; // Number of voxels for one side of the cube: 2**numLevels
    float voxelWidth; // The length of one voxel in world space: width / dimension
    void** levels; // Array of SVONode*'s that correspond to the levels of the SVO with 0 as root
    unsigned int* levelSizes;



      // void setVoxel(unsigned int x, unsigned int y, unsigned int z, uint64_t* activeNodes, uint64_t* nodes);
      // void voxelizeTriangle(const Triangle& triangle, uint64_t* activeNodes, uint64_t* nodes);
      // bool isSet(unsigned int x, unsigned int y, unsigned int z);
      // bool isNodeNotEmpty(SVONode *node);
      // bool isChildSet(SVONode *node, unsigned int i);
      // bool isLeafSet(uint64_t* node, unsigned int i);
      // void printBinary();
      // void writeImages();
      // unsigned int countAtLevel(unsigned int level);
      // string getMemorySize(unsigned int size);
      

      // SVONode* root;
      //tbb::concurrent_unordered_map<unsigned int, unsigned int>* voxelTriangleIndexMap;
      // uint64_t sizeWithoutMaterials;
};


#endif