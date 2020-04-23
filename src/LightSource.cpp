#include "LightSource.h"
#include "Parse.h"
#include <iostream>
#include <string.h>
#include <sstream>
using namespace std;

LightSource::LightSource(glm::vec3 location, glm::vec3 color) :
   location(location),
   color(color)
{
}

LightSource *LightSource::parse(ifstream &infile, stringstream &s)
{
	cout << "parsing light_source" << endl;
	vec3 location = Parse::Vector(s);
	vec3 color = Parse::Vector(s);
	LightSource *light = new LightSource(location, color);
	return light;
}