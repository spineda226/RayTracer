#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <time.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Image.h"

#include "Parse.h"
#include "Point.h"
#include "Shape.h"
#include "Sphere.h"
#include "BVH_Node.h"
#include "LightSource.h"
#include "Camera.h"
#include "Vertex.h"
#include "Color.h"
#include "Box.h"
#include "Triangle.h"
#include "Intersection.h"
#include "Plane.h"
#include "Data.h"
#include "Raytrace.h"
#include "Triangle.h"
#include "SVO.h"
//#include <easy/profiler.h>

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;
using namespace chrono;
using namespace glm;

int g_width, g_height, test_x, test_y;
int testMode = 0;
string file_name;

int main(int argc, char **argv)
{
   //EASY_PROFILER_ENABLE;

   if(argc != 4 and argc != 7) 
   {
      cout << "Usage: ./raytrace <width> <height> filename" << endl;
      return 0;
   }
   if (argc == 4) {
      g_width = atoi(argv[1]);
      g_height = atoi(argv[2]);
      file_name = argv[3];
   }
   else if (argc == 7) {
      testMode = 1;
      g_width = atoi(argv[2]);
      g_height = atoi(argv[3]);
      test_x = atoi(argv[4]);
      test_y = atoi(argv[5]);
      file_name = argv[6];
   }

   unsigned int numLevels = 3; // Brent
   
   // Timer
   struct timespec start, finish;
   double elapsed;

   // Scene Object List
   vector<Triangle *> triangles;
   vector<Shape *> planes;
   Camera *camera;
   vector<LightSource *> lights;

   // Parse the file
   Parse::parse_file(file_name, &triangles, &planes, &camera, &lights);

   AABB *meshBBox = triangles[0]->calculateBBox(); // testing with a single triangle first
   SVO svo(numLevels, *meshBBox, &triangles); // Brent

   // BVH_Node::sort_objects_on_axis(&objects, 0, objects.size()-1, 0);
   // BVH_Node *bvh = new BVH_Node();
   // cout << "Object size: " << objects.size() << endl;
   // bvh->recursive_tree_build(&objects, 0, objects.size()-1, 0);
   
   // //for (Shape *s : objects)
   // //{
   // //   std::cout << "Shape Center: " << s->getCenter().x << " " << s->getCenter().y << " " << s->getCenter().z << std::endl;
   // //}
   
   // clock_gettime(CLOCK_MONOTONIC, &start);
   // if (testMode)
   // {
   //    //single_raytrace_without_spatial(g_width, g_height, test_x, test_y, objects, planes, *camera, lights);
   //    single_raytrace(g_width, g_height, test_x, test_y, *bvh, planes, *camera, lights);
   // }
   // else
   // {
   //    //raytrace_without_spatial(g_width, g_height, test_x, test_y, objects, planes, *camera, lights);
   //    raytrace(g_width, g_height, *bvh, planes, *camera, lights);
   // }
   // clock_gettime(CLOCK_MONOTONIC, &finish);
   // elapsed = (finish.tv_sec - start.tv_sec);
   // elapsed += (finish.tv_nsec - start.tv_nsec)/1000000000.0;
   // cout << "Elapsed Time: " << elapsed << " seconds" << endl;
   // profiler::dumpBlocksToFile("test_profile.prof");
   return 0;
   
}
