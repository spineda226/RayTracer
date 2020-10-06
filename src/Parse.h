#ifndef _PARSE_H_
#define _PARSE_H_

#include <string>
#include <sstream>
#include <vector>

#include "Camera.h"
#include "AABB.h"
#include "LightSource.h"
class Triangle;
class Shape;

class Parse
{
	public:	
		Parse() {};
		~Parse() {};
		static AABB *parse_file(std::string filename, std::vector<Triangle *> *triangles, std::vector<Shape *> *planes, Camera **camera, std::vector<LightSource *> *lights);
		static vec3 Vector(std::stringstream & Stream);
		static vec4 Vector4(std::stringstream & Stream);
		static void Modifiers(Shape *shape, std::ifstream &infile);
		static void Pigment(Shape *shape, std::stringstream &s);
		static void Finish(Shape *shape, std::stringstream &s);
};

#endif