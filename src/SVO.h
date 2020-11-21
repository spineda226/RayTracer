/* Class for Sparse Voxel Octrees
 *  Modified from Brent Williams
 */

#ifndef _SVO_H_
#define _SVO_H_

#include "Triangle.h"
#include <glm/glm.hpp>

#include "AABB.h"
#include "SVONode.h"
#include "Ray.h"
//#include <vector>
//#include <stdint.h>
#include "Voxels.h"
//#include <string>
#include <unordered_map>
#include "tbb/concurrent_unordered_map.h"
#include "ShadingData.h"

#include <chrono>

#define SET_8_BITS 255

using namespace std;
using namespace glm;

class SVO
{
  public:
    SVO(const unsigned int numLevels, const AABB& boundingBoxVal, const std::vector<Triangle *> *triangles, std::vector<PhongMaterial> &materials);
		~SVO() {};

    void build(const std::vector<Triangle *> *triangles);

    bool intersect(const Ray& ray, float& t, vec3& normal, uint64_t& voxelIndex);
    bool intersect(const Ray& ray, float& t, SVONode* node, unsigned int level, AABB aabb, vec3& normal, uint64_t& voxelIndex);
    int getData(unsigned int x, unsigned int y, unsigned int z);
    unsigned int getNumLevels() const { return numLevels; }
    tbb::concurrent_unordered_map<unsigned int, ShadingData>* getVoxelNormalMap() const { return voxelNormalMap; }
    tbb::concurrent_unordered_map<unsigned int, unsigned int>* getTriangleMap() const { return voxelTriangleIndexMap; }
    PhongMaterial getMaterial(unsigned int i) const { return materials[i]; }

	private:
    unsigned int numLevels; // depth (root is level 0), needs to be at least 3 levels
		AABB boundingBox;
		unsigned long size; // Total number of voxels if the SVO was full: 8**numLevels
    unsigned int dimension; // Number of voxels for one side of the cube: 2**numLevels
    float voxelWidth; // The length of one voxel in world space: width / dimension
    void** levels; // Array of SVONode*'s that correspond to the levels of the SVO with 0 as root (last level )
    unsigned int* levelSizes; 

    tbb::concurrent_unordered_map<unsigned int, unsigned int>* voxelTriangleIndexMap;
    bool isNodeNotEmpty(SVONode *node);
    SVONode* root;
    string getMemorySize(unsigned int size);

    bool isLeafSet(uint64_t* node, unsigned int i);
    bool isChildSet(SVONode *node, unsigned int i); // check later
    uint64_t getLevelIndexSum(unsigned int level, unsigned int index);

    // Material
    tbb::concurrent_unordered_map<unsigned int, ShadingData>* voxelNormalMap;
    Voxels *leafVoxels;
    std::vector<PhongMaterial> materials;





      // void setVoxel(unsigned int x, unsigned int y, unsigned int z, uint64_t* activeNodes, uint64_t* nodes);
      // void voxelizeTriangle(const Triangle& triangle, uint64_t* activeNodes, uint64_t* nodes);
      // bool isSet(unsigned int x, unsigned int y, unsigned int z);
      


      // void printBinary();
      // void writeImages();
      // unsigned int countAtLevel(unsigned int level);
      
    
      // uint64_t sizeWithoutMaterials;
};


#endif