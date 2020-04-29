#include <iostream>
#include "Plane.h"
#include "Intersection.h"
#include "Parse.h"
#include <string.h>
#include <sstream>
#include <stdlib.h> //strtof

Plane::Plane(vec3 normal, float distance, vec3 color, properties *finish) :
   normal(normal),
   distance(distance),
   Shape(color, finish)
{
}

Plane *Plane::parse(std::ifstream &infile, std::stringstream &s)
{
	std::string d, line;
	//std::cout << "parsing plane" << std::endl;
	vec3 normal = Parse::Vector(s);
	s.ignore(std::numeric_limits<std::streamsize>::max(), ',');
	s >> d;
	float distance = strtof(d.c_str(), NULL);
	//std::cout << "distance: " << distance << std::endl;
	Plane *plane = new Plane(normal, distance);
	Parse::Modifiers(plane, infile);
	return plane;
}

// return -1 if no intersection
// updates the ray's firstHit only if it is closer
int Plane::getClosestIntersection(Ray &ray)
{
	vec3 rayDir = ray.getDir();

	// Equation to test for intersection: t = (D -  dot(p_0, n)) / (dot(d, n))
	vec3 n = normalize(normal);
	if (dot(rayDir, n) == 0) // no intersection
		return -1;
	float solution = (distance - dot(ray.getPoint(), n))/(dot(rayDir, n));
	if (solution < 0)
		return -1;

    // Check if intersection is the closest
    if (ray.getIntersection().hasIntersection() == 0) // first one
    	ray.setIntersection(Intersection(1, solution, ray.getPoint() + solution*ray.getDir(), this));
    else if (solution < ray.getIntersection().getDistance()) // check if it is closer than the saved one
    	ray.setIntersection(Intersection(1, solution, ray.getPoint() + solution*ray.getDir(), this));
	return 0;
}

AABB *Plane::calculateBBox()
{
	AABB *box = new AABB();
	box->reset(vec3(0));
	return box;
}

