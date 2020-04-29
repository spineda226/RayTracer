#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

#include <stdio.h>
#include <vector>
#include "Shape.h"
#include "Camera.h"
#include "LightSource.h"

#include <glm/glm.hpp>
using namespace glm;

void raytrace(int g_width, int g_height, int x, int y, const std::vector<Shape *> &objects, const Camera &camera, const std::vector<LightSource *> &lights);
void single_raytrace(int g_width, int g_height, int x, int y, const std::vector<Shape *> &objects, const Camera &camera, const std::vector<LightSource *> &lights);
vec3 raycolor(const std::vector<Shape *> &objects, const std::vector<LightSource *> &lights, const Ray &r);

#endif
