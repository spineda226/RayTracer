#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Image.h"

#include "Parse.h"
#include "Point.h"
#include "Shape.h"
#include "Sphere.h"
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

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;
using namespace glm;

int g_width, g_height, test_x, test_y;
int testMode = 0;

int main(int argc, char **argv)
{
   if(argc != 3 and argc != 6) 
   {
      cout << "Usage: ./raytrace <width> <height>" << endl;
      return 0;
   }
   if (argc == 3) {
      g_width = atoi(argv[1]);
      g_height = atoi(argv[2]);
   }
   else if (argc == 6) {
      testMode = 1;
      g_width = atoi(argv[2]);
      g_height = atoi(argv[3]);
      test_x = atoi(argv[4]);
      test_y = atoi(argv[5]);
   }
   
   // Scene Object List
   vector<Shape *> objects;
   Camera *camera;
   vector<LightSource *> lights;

   // Parse the file
   Parse::parse_file("../resources/simple.pov", &objects, &camera, &lights);

   /*
   stringstream s;
   vec3 v;

   s.str("<3.0>");
   v = Parse::Vector(s);
   cout << v.x << " " << v.y << " " << v.z << endl;

   s.str("<3.0, 1.0, 2.0>");
   v = Parse::Vector(s);
   cout << v.x << " " << v.y << " " << v.z << endl;

   s.str("<3.0, 1.0 2.0>");
   v = Parse::Vector(s);
   cout << v.x << " " << v.y << " " << v.z << endl;

   */
   //init_sphere_pov(&objects, &camera, &lights);
   //init_simple_pov(&objects, &camera, &light);
   //init_simple_cam_pov(&objects, &camera, &lights, 2);
   //init_simple_tri_pov(&objects, &camera, &lights);

   /*
   if (testMode)
      single_raytrace(g_width, g_height, test_x, test_y, objects, *camera, lights);
   else
      raytrace(g_width, g_height, test_x, test_y, objects, *camera, lights);
   return 0;
   */
}
