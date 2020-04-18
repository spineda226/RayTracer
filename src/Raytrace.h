#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

#include <stdio.h>
#include <vector>
using namespace std;
#include "Shape.h"
#include "Camera.h"
#include "LightSource.h"

#include <glm/glm.hpp>
using namespace glm;

void raytrace(int g_width, int g_height, int x, int y, const vector<Shape *> &objects, const Camera &camera, const LightSource &light);
void single_raytrace(int g_width, int g_height, int x, int y, const vector<Shape *> &objects, const Camera &camera, const LightSource &light);
vec3 raycolor(const vector<Shape *> &objects, const LightSource &light, const Ray &r);

#endif
