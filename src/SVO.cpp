/**
 * Modified from Brent Williams
 */

#include "SVO.h"
#include <iostream> // cerr
#include "Voxels.h"

/**
 * Instantiates and initializes the SVO.
 */
SVO::SVO(const unsigned int numLevels, AABB &boundingBox, const std::vector<Triangle *> *triangles)
 : numLevels(numLevels),
   boundingBox(boundingBox),
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
   
   boundingBox.square();
   voxelWidth = (boundingBox.getMax().x - boundingBox.getMin().x) / dimension;
   levels = new void*[numLevels-1]; // has the -1 because the last two levels are uint64's 
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
   // auto start = chrono::steady_clock::now();
   Voxels* leafVoxels = new Voxels(numLevels, boundingBox, triangles);
   // auto end = chrono::steady_clock::now();
   // auto diff = end - start;
   // cout << "\t\tTime Voxelization: " << chrono::duration <double, milli> (diff).count() << " ms" << endl;

   // auto svoStartTime = chrono::steady_clock::now();
   // uint64_t* leafVoxelData = leafVoxels->data;
   // unsigned int numLeafs = leafVoxels->dataSize;

   // voxelTriangleIndexMap = leafVoxels->voxelTriangleIndexMap;
   
   // // std::cout << "levels: " << numLevels << "\n";
   // // std::cout << "Number of leaf nodes: " << numLeafs << "\n";

   // // Save the pointer to the leaf nodes
   // unsigned int currentLevel = numLevels-2;
   // levels[currentLevel] = (void*)leafVoxelData; 
   // cerr << "Leaf Level = " << currentLevel << endl;
   // currentLevel--;
   
   // if (numLevels <= 3)
   // {
   //    std::cerr << "CANNOT build SVO with levels <= 3.\nExitting...\n";
   //    exit(EXIT_FAILURE);
   // }
   
   // int numPrevLevelNodes = numLeafs; //512
   // int numCurrLevelNodes = numPrevLevelNodes / 8; //64
   // SVONode* prevLevelNodes;
   // SVONode* currLevelNodes; 
   // currLevelNodes = new SVONode[numCurrLevelNodes](); 
   // levelSizes[numLevels-2] = 0;

   // // Set the level above the leaf nodes
   // for (int i = 0; i < numPrevLevelNodes; i++)
   // {
   //    if (leafVoxelData[i] > 0)
   //    {
   //       currLevelNodes[i/8].childPointers[i%8] = (void *)  &(leafVoxelData[i]);
   //       levelSizes[numLevels-2]++;
   //    }
   // }

   // levels[currentLevel] = (void*)currLevelNodes;
   // //levelSizes[currentLevel] = numCurrLevelNodes * sizeof(SVONode);
   // currentLevel--;
   
   // // Set the rest of the non leaf nodes
   // while(numCurrLevelNodes >= 64)
   // {
   //    numPrevLevelNodes = numCurrLevelNodes; //64
   //    numCurrLevelNodes /= 8; // 8
   //    prevLevelNodes = currLevelNodes;
   //    currLevelNodes = new SVONode[numCurrLevelNodes];
   //    //levelSizes[currentLevel] = numCurrLevelNodes * sizeof(SVONode);
   //    levelSizes[currentLevel+1] = 0;
      
   //    // For each of the previous level's nodes we set the child pointers
   //    for (int i = 0; i < numPrevLevelNodes; i++)
   //    {
   //       if (isNodeNotEmpty(&prevLevelNodes[i]))
   //       {
   //          currLevelNodes[i/8].childPointers[i%8] = (void *)  &(prevLevelNodes[i]);
   //          levelSizes[currentLevel+1]++;
   //       }
   //    }

   //    // Save the pointer for each level's nodes in the level's array
   //    levels[currentLevel] = currLevelNodes;
   //    currentLevel--;
   // }
   
   // // Set the root node
   // prevLevelNodes = currLevelNodes;
   // root = new SVONode;
   // levelSizes[currentLevel+1] = 0;
   // for (int i = 0; i < 8; i++)
   // {
   //    if (isNodeNotEmpty(&prevLevelNodes[i]))
   //    {
   //       root->childPointers[i] = (void *)  &(prevLevelNodes[i]);
   //       levelSizes[currentLevel+1]++;
   //    }
   // }

   // // Save the pointer for root node in the levels array
   // levels[currentLevel] = root;
   // levelSizes[currentLevel] = 1;

   // uint64_t totalSVOMemory = 0;
   // cout << "SVO Size: " << endl;
   // for (int i = 0; i < numLevels-2; ++i)
   // {
   //    cout << "[" << i << "]: " << levelSizes[i] << endl;
   //    totalSVOMemory += levelSizes[i] * sizeof(SVONode);
   // }
   // cout << "[" << numLevels-2 << "]: " << levelSizes[numLevels-2] << endl;
   // totalSVOMemory += levelSizes[numLevels-2] * sizeof(uint64_t);


   // cout << "SVO (without materials) Memory Size: " << totalSVOMemory << " (" << getMemorySize(totalSVOMemory) << ")" << endl;
   
   // auto svoEndTime = chrono::steady_clock::now();
   // auto svoDiff = svoEndTime - svoStartTime;
   // cout << "\t\tTime SVO Building: " << chrono::duration <double, milli> (svoDiff).count() << " ms" << endl;
}