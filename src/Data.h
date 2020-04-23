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

void init_sphere_pov(std::vector<Shape *> *objects, Camera **camera, std::vector<LightSource *> *lights);
void init_simple_pov(std::vector<Shape *> *objects, Camera **camera, std::vector<LightSource *> *lights);
void init_simple_cam_pov(std::vector<Shape *> *objects, Camera **camera, std::vector<LightSource *> *lights, int option);
void init_simple_tri_pov(std::vector<Shape *> *objects, Camera **camera, std::vector<LightSource *> *lights);

#endif
