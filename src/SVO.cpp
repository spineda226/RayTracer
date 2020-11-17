/**
 * Modified from Brent Williams
 */

#include "SVO.h"
#include <iostream> // cerr
#include "Voxels.h"
#include "MortonCode.h"

/**
 * Instantiates and initializes the SVO.
 */
SVO::SVO(const unsigned int numLevels, const AABB &boundingBoxVal, const std::vector<Triangle *> *triangles)
 : numLevels(numLevels),
   boundingBox(boundingBoxVal),
   size(pow(8, numLevels)), 
   dimension(pow(2, numLevels)),
   voxelWidth(0)
{
   if (numLevels <= 2)
   {
      std::string err("\nNumber of levels too small\n");
      std::cerr << err;
      throw std::out_of_range(err);
   }
   
   boundingBox.square(); // turns bounding box into a cube with same side lengths
   voxelWidth = (boundingBox.getMax().x - boundingBox.getMin().x) / dimension; // width of one voxel
   // Array of void* | has the -1 because the last two levels are uint64s
   levels = new void*[numLevels-1]; // has the -1 because the last two levels are uint64's
   //| check later: should this be -2? -- later: no it should be -1
   levelSizes = new unsigned int[numLevels-1]();
   build(triangles);


}

/**
 * Build the SVO that works for >= 4 levels.
 *
 * Tested: 2-16-2014 
 */
void SVO::build(const std::vector<Triangle *> *triangles)
{
   auto start = chrono::steady_clock::now();
   leafVoxels = new Voxels(numLevels, boundingBox, triangles);
   auto end = chrono::steady_clock::now();
   auto diff = end - start;
   cout << "\t\tTime Voxelization: " << chrono::duration <double, milli> (diff).count() << " ms" << endl;

   auto svoStartTime = chrono::steady_clock::now(); 
   uint64_t* leafVoxelData = leafVoxels->getData(); // each uint64_t is stored contiguously in dynamic memory
   unsigned int numLeafs = leafVoxels->getDataSize(); // A 2-level tree has one leaf

   voxelTriangleIndexMap = leafVoxels->getVoxelTriangleIndexMap();
   voxelNormalMap = leafVoxels->getVoxelNormalMap();

   std::cout << "levels: " << numLevels << "\n";
   std::cout << "Number of leaf nodes: " << numLeafs << "\n";

   // Save the pointer to the leaf nodes
   unsigned int currentLevel = numLevels-2; 
   levels[currentLevel] = (void*)leafVoxelData; 
   cerr << "Leaf Level = " << currentLevel << endl;
   currentLevel--;
   
   if (numLevels <= 3) // Verified 10/10/20 : needs to be at least 4 levels because need leaf levels, at least 1 SVO* level, and root
   {
      std::cerr << "CANNOT build SVO with levels <= 3.\nExitting...\n";
      exit(EXIT_FAILURE);
   }
   
   int numPrevLevelNodes = numLeafs; //5: 512
   int numCurrLevelNodes = numPrevLevelNodes / 8; //5: 64
   SVONode* prevLevelNodes;
   SVONode* currLevelNodes; 
   currLevelNodes = new SVONode[numCurrLevelNodes](); 
   levelSizes[numLevels-2] = 0; // initialize leaf level size to 0 | should store number of filled leaves
   // Set the level above the leaf nodes (a leaf node is a uint64_t)
   for (int i = 0; i < numPrevLevelNodes; i++)
   {
      if (leafVoxelData[i] > 0) // if at least one voxel is filled out of the 64, then set the current node's child pointer 
      {
         currLevelNodes[i/8].childPointers[i%8] = (void *)  &(leafVoxelData[i]);
         levelSizes[numLevels-2]++; // increment filled leaf counter for bottom level
      }
      // else it is empty space so keep the child pointer of the current node to Null in that spot
      // check later: do we need to/can we free this space?
   }

   levels[currentLevel] = (void*)currLevelNodes; // block of SVONode * on that level
   currentLevel--;
   
   // Set the rest of the non leaf nodes
   while(numCurrLevelNodes >= 64)
   {
      numPrevLevelNodes = numCurrLevelNodes; //64
      numCurrLevelNodes /= 8; // 8
      prevLevelNodes = currLevelNodes;
      currLevelNodes = new SVONode[numCurrLevelNodes];
      //levelSizes[currentLevel] = numCurrLevelNodes * sizeof(SVONode);
      levelSizes[currentLevel+1] = 0;
      
      // For each of the previous level's nodes we set the child pointers
      for (int i = 0; i < numPrevLevelNodes; i++)
      {
         if (isNodeNotEmpty(&prevLevelNodes[i]))
         {
            currLevelNodes[i/8].childPointers[i%8] = (void *)  &(prevLevelNodes[i]);
            levelSizes[currentLevel+1]++;
         }
      }

      // Save the pointer for each level's nodes in the level's array
      levels[currentLevel] = currLevelNodes;
      currentLevel--;
   }
   cout << "Current Level at the End: " << currentLevel << endl;
   
   // Set the root node
   prevLevelNodes = currLevelNodes;
   root = new SVONode(); // changed this steven
   levelSizes[currentLevel+1] = 0;
   for (int i = 0; i < 8; i++)
   {
      if (isNodeNotEmpty(&prevLevelNodes[i]))
      {
         root->childPointers[i] = (void *)  &(prevLevelNodes[i]);
         levelSizes[currentLevel+1]++;
      }
   }

   // Save the pointer for root node in the levels array
   levels[currentLevel] = root; // will only be one SVONode * (not a block)
   levelSizes[currentLevel] = 1;

   uint64_t totalSVOMemory = 0;
   cout << "SVO Size: " << endl;
   for (int i = 0; i < numLevels-2; ++i)
   {
      cout << "[" << i << "]: " << levelSizes[i] << endl;
      totalSVOMemory += levelSizes[i] * sizeof(SVONode);
   }
   cout << "[" << numLevels-2 << "]: " << levelSizes[numLevels-2] << endl;
   totalSVOMemory += levelSizes[numLevels-2] * sizeof(uint64_t);


   cout << "SVO (without materials) Memory Size: " << totalSVOMemory << " (" << getMemorySize(totalSVOMemory) << ")" << endl;
   
   auto svoEndTime = chrono::steady_clock::now();
   auto svoDiff = svoEndTime - svoStartTime;
   cout << "\t\tTime SVO Building: " << chrono::duration <double, milli> (svoDiff).count() << " ms" << endl;
}

/**
 * Returns a boolean indicating whether all the node's children are not empty.
 *
 * Tested: 1-16-2015
 */
bool SVO::isNodeNotEmpty(SVONode *node)
{
   for (int i = 0; i < 8; i++)
   {
      if (node->childPointers[i] != NULL)
      {
         return true;
      }
   }

   return false;
}

string SVO::getMemorySize(unsigned int size)
{
   string b = " B";
   string kb = " KB";
   string mb = " MB";
   string gb = " GB";
   string units[] = {b,kb,mb,gb};
   float currentSize = (float)size;
   float lastSize = (float)size;
   int i;

   for (i = 0; i < 4 && currentSize > 1.0f; ++i)
   {
      lastSize = currentSize;
      currentSize /= 1024.0f;
   }

   return to_string(lastSize) + units[i-1];
}

/**
 * Returns a boolean indicating whether the node's leaf at the ith bit is set.
 *
 * Tested: 1-27-2014 
 */
bool SVO::isLeafSet(uint64_t* node, unsigned int i)
{
   uint64_t leaf = (uint64_t) *node;
   uint64_t toAnd = (1L << i);
   return (leaf & toAnd) > 0; 
}

/**
 * Returns a boolean indicating whether the node's child is set.
 *
 * Tested: 1-16-2015 
 */
bool SVO::isChildSet(SVONode *node, unsigned int i)
{
   return node->childPointers[i] != NULL;
}

uint64_t SVO::getLevelIndexSum(unsigned int level, unsigned int index)
{
   uint64_t sum = 0;
   uint64_t perChild = 1;

   for (unsigned int i = 0; i < (numLevels - level - 1); i++)
   {
      perChild *= 8;
   }
   sum = perChild * index;

   return sum;
}

/**
 * Returns a boolean indicating whether the node's child is set.
 *
 * Tested: 
 */
// bool SVO::isChildSet(void* node, unsigned int i)
// {
//    uint64_t toAnd = 1L << i;
//    uint64_t mask = *((uint64_t*) node);
//    //cout << "\t\t(" << mask <<  " & " << toAnd << ") = " << (mask | toAnd) << endl;
//    return (mask & toAnd) != 0;
// }

/**
 * Returns whether the ray provided intersects the DAG and t the distance along the ray
 *
 * Tested: 
 */
bool SVO::intersect(const Ray& ray, float& t, vec3& normal, uint64_t& voxelIndex)
{
   vec3 bbMins = boundingBox.getMin();
   vec3 bbMaxs = boundingBox.getMax();

   vec3 mins(bbMins.x, bbMins.y, bbMins.z);
   vec3 maxs(bbMaxs.x, bbMaxs.y, bbMaxs.z);
   AABB aabb(mins, maxs);
   return intersect(ray, t, root, 0, aabb, normal, voxelIndex);
}

/**
 * Resursive intersection method that returns whether the ray provided intersects the DAG and t the
 * distance along the ray.
 *
 * Tested: 
 */
bool SVO::intersect(const Ray& ray, float& t, SVONode* node, unsigned int level, AABB aabb, vec3& normal, uint64_t& voxelIndex)
{
   //Child values by index based on morton encoding
   vec3 childOffsets[8] = { 
      glm::vec3(0, 0, 0),
      glm::vec3(1, 0, 0),
      glm::vec3(0, 1, 0),
      glm::vec3(1, 1, 0),
      glm::vec3(0, 0, 1),
      glm::vec3(1, 0, 1),
      glm::vec3(0, 1, 1),
      glm::vec3(1, 1, 1) };


   vec3 mins = aabb.getMin();
   vec3 maxs = aabb.getMax();
   vec3 uselessNormal;
   uint64_t finalVoxelIndex = 0;

   //cout << "\tTraversing level " << level << endl;

   // node is not a leaf node
   if (level < numLevels-2)
   {
      // If the parent node is hit
      if (aabb.intersect(ray, t, uselessNormal)) // does same thing as my hit
      //if (aabb.hit(ray, t))
      {
         //cout << "\tNode hit." << endl;
         float newDim = (maxs.x - mins.x) / 2.0f;
         bool isHit = false;
         t = FLT_MAX;
         //finalVoxelIndex = 0;
         
         for (unsigned int i = 0; i < 8; i++)
         {
            if (isChildSet(node, i))
            {
               //cout << "\tChild " << i << " is set." << endl;
               vec3 newMins(mins + (childOffsets[i] * newDim));
               vec3 newMaxs(newMins.x + newDim, newMins.y + newDim, newMins.z + newDim);
               AABB newAABB(newMins, newMaxs);
               
               //cout << "\tNew AABB: ";
               //newAABB.print();
               //cout <<  "\tIntersecting with child..." << endl << endl;
               
               //uint64_t emptyCount = getEmptyCount((void*)node,i);
               uint64_t levelIndexSum = getLevelIndexSum(level,i);
               uint64_t tempVoxelIndex = voxelIndex + levelIndexSum;

               // cout << "Level " << level << ", child = " << i << endl;
               // cout << "\ttempMoxelIndex = moxelIndex + levelIndexSum;" << endl;
               // cout << "\t" << tempVoxelIndex << " = " << voxelIndex << " + " << levelIndexSum << endl << endl;

               float newT = 0.0f;
               bool newHit = intersect(ray, newT, (SVONode *)node->childPointers[i], level+1, newAABB, normal, tempVoxelIndex);
               //cout << "\n\tChild " << i << " hit: " << newHit << endl;

               if (newHit && newT < t)
               {
                  t = newT;
                  finalVoxelIndex = tempVoxelIndex;
               }
               isHit = isHit || newHit;
            }
         }
         voxelIndex = finalVoxelIndex;  
         return isHit;
      }
      // If the parent node is not hit
      else
      {
         return false;
      }
   }
   // node is a leaf node
   else
   {
      float newDim = (maxs.x - mins.x) / 4.0f;
      t = FLT_MAX;
      bool isHit = false;
      //finalVoxelIndex = 0;

      // Go through each of the 64 child nodes stored in the given leaf
      for (unsigned int i = 0; i < 64; i++)
      {
         //cout << "i: " << i << "\n";
         // If the leaf is not empty
         if (isLeafSet((uint64_t*)node, i))
         {
            unsigned int x, y, z;
            mortonCodeToXYZ((uint32_t)i, &x, &y, &z, 2);
            vec3 offset((float)x,(float)y,(float)z);
            vec3 newMins(mins + (offset * newDim));
            vec3 newMaxs(newMins.x + newDim, newMins.y + newDim, newMins.z + newDim);
            AABB newAABB(newMins, newMaxs);
            float newT = 0.0f;
            vec3 tempNormal;
            bool newHit = newAABB.intersect(ray,newT, tempNormal);
            //bool newHit = newAABB.hit(ray,newT);

            //uint64_t emptyCount = getLeafNodeEmptyCount( *((uint64_t*)node), i);
            uint64_t levelIndexSum = i; // Beacause at the last 2 levels it is just the # to the left of it which in this case is the index within the uint64_t
            uint64_t tempVoxelIndex = voxelIndex + levelIndexSum;

            if (newHit && newT < t)
            {
               // cout << "Level " << level << ", child = " << i << endl;
               // cout << "\ttempVoxelIndex = voxelIndex + levelIndexSum;" << endl;
               // cout << "\t" << tempVoxelIndex << " = " << voxelIndex << " + " << levelIndexSum  << endl << endl;

               t = newT;
               normal = tempNormal;
               finalVoxelIndex = tempVoxelIndex;
            }
            isHit = isHit || newHit;
         }
      }
      voxelIndex = finalVoxelIndex;
      return isHit;
   }
}

/**
 * gets the voxel at the given x, y and z values (return 0 or 1)
 *
 * Tested: 9-3-2013 
 * check later: mutex deprecated?
 */
int SVO::getData(unsigned int x, unsigned int y, unsigned int z)
{
   //If each individual voxel had an index, the voxeNumber is that index
   unsigned int voxelNumber = mortonCode(x,y,z,numLevels);
   //std::cout << "Voxelization Voxel Number: " << voxelNumber << "\n";
   //unsigned int voxelNumber = x + dimension*y + dimension*dimension*z;
   
   //dataIndex is the index into the uint64 array of the current voxel
   unsigned int dataIndex = voxelNumber / 64;
   
   //bitIndex is the current voxel (represented by a bit) to set
   unsigned int bitIndex =  voxelNumber % 64;
   
   //The mask used to set the voxel
   uint64_t toAnd = (1L << bitIndex);
   uint64_t *data = (uint64_t *)levels[numLevels-2];
   if (data[dataIndex] & toAnd) {
      return 1;
   }
   return 0;
   
   // if (data[dataIndex] | )



   // tbb::mutex::scoped_lock lock;
   // lock.acquire(sMutex);
   // //#pragma omp atomic
   // data[dataIndex] |= toOr; // sets the bitIndex bit 
   // lock.release();
}
