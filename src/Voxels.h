/**
 * Voxels.hpp
 * 
 * Represents a set of sparse voxels. 
 *
 * Note:
 * Only supports voxels 
 *
 * Modified from Brent Williams
 */

#ifndef _VOXELS_H
#define _VOXELS_H

// #include <stdio.h>
// #include <stdint.h>
// #include <math.h>
// #include <iostream>
// #include <stdexcept>
// #include <string>
// #include <vector>
// #include <sstream>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <iomanip>

// OpenMP
#include "/usr/local/Cellar/libomp/10.0.1/include/omp.h"

#include "AABB.h"
#include "Triangle.h"
// #include "Intersect.hpp"
// #include "Image.hpp"
//#include <unordered_map>
#include <tbb/concurrent_unordered_map.h>
#include "tbb/mutex.h"
// #include "tbb/atomic.h"
// #include "tbb/tbb.h"


class Voxels
{
   public:
      Voxels(const unsigned int numLevels, AABB &boundingBox, const std::vector<Triangle *> *triangles);
      ~Voxels();

      // Getters
      uint64_t *getData() const { return data; }
      unsigned int getDataSize() const { return dataSize;}

      unsigned int calculateDataSize(unsigned int levels);
      void build(const std::vector<Triangle *> *triangles);
      void voxelizeTriangle(const Triangle& triangle, unsigned int i);
      void set(unsigned int x, unsigned int y, unsigned int z);
      
      // bool isSet(unsigned int x, unsigned int y, unsigned int z);
      // unsigned int countSetVoxels();
      // void printBinary();
      // void writeImages();
      // bool cacheExists(std::string fileName);
      // void writeVoxelCache(std::string fileName);
      // std::string getFileNameFromPath(std::string fileName);
      
      // uint64_t& operator[](unsigned int i);
      
      // unsigned long getSize() const;

   private:
      uint64_t *data;
      AABB boundingBox;
      unsigned int levels;
      unsigned long size; // Total number of voxels 
      unsigned int dimension; // Number of voxels for one side of the cube
      float voxelWidth; // The length of one voxel in world space
      unsigned int dataSize; // The number of uint64_t allocated to data
      tbb::concurrent_unordered_map<unsigned int, unsigned int>* voxelTriangleIndexMap; // maps morton index to triangle index in vector
      tbb::mutex sMutex; // protect data being written into data buffer
};

// void binaryToString(uint64_t data, char* str);
// unsigned int countSetBits(uint64_t data);

#endif