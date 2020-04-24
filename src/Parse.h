#ifndef _PARSE_H_
#define _PARSE_H_

#include <string>
#include <sstream>
#include <vector>

#include "Camera.h"
#include "LightSource.h"
class Shape;

class Parse
{
	public:	
		Parse() {};
		~Parse() {};
		static void parse_file(std::string filename, std::vector<Shape *> *objects, Camera **camera, std::vector<LightSource *> *lights);
		static vec3 Vector(std::stringstream & Stream);
		static void Modifiers(Shape *shape, std::ifstream &infile);
		static void Pigment(Shape *shape, std::stringstream &s);
		static void Finish(Shape *shape, std::stringstream &s);
};

#endif