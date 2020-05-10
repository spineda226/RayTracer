#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

#include <stdio.h>
#include <vector>
#include "Shape.h"
#include "Camera.h"
#include "LightSource.h"
#include "BVH_Node.h"

#include <glm/glm.hpp>
using namespace glm;

void raytrace(int g_width, int g_height, const BVH_Node &bvh, const std::vector<Shape *> &planes, const Camera &camera, const std::vector<LightSource *> &lights);
vec3 recursive_raytrace(const BVH_Node &bvh, const std::vector<Shape *> &planes, const std::vector<LightSource *> &lights, Ray &ray, int iteration, float ratio=1, bool entering=true);
void single_raytrace(int g_width, int g_height, int x, int y, const BVH_Node &bvh, const std::vector<Shape *> &planes, const Camera &camera, const std::vector<LightSource *> &lights);
vec3 raycolor(const BVH_Node &bvh, const std::vector<Shape *> &planes, const std::vector<LightSource *> &lights, const Ray &r);

Ray& createReflect(const Ray &incident);
Ray& createRefract(const Ray &incident, bool entering=true, float ratio=1);


// Without spatial data structure optimization
void raytrace_without_spatial(int g_width, int g_height, int x, int y, const std::vector<Shape *> &objects, const std::vector<Shape *> &planes,
	                          const Camera &camera, const std::vector<LightSource *> &lights);
void single_raytrace_without_spatial(int g_width, int g_height, int x, int y, const std::vector<Shape *> &objects,
	                                 const std::vector<Shape *> &planes, const Camera &camera, const std::vector<LightSource *> &lights);
vec3 raycolor_without_spatial(const std::vector<Shape *> &objects, const std::vector<Shape *> &planes, const std::vector<LightSource *> &lights, const Ray &r);

#endif
