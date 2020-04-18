#ifndef _DATA_H_
#define _DATA_H_

#include <vector>
#include "Camera.h"
#include "LightSource.h"
class Shape;

struct properties {
   float ambient;
   float diffuse;
   float specular;
   float roughness;
   float reflection;
   float refraction;
   float ior;
 };

void init_sphere_pov(std::vector<Shape *> *objects, Camera **camera, LightSource **light);
void init_simple_pov(std::vector<Shape *> *objects, Camera **camera, LightSource **light);
void init_simple_cam_pov(std::vector<Shape *> *objects, Camera **camera, LightSource **light, int option);

#endif
