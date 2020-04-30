#include "Box.h"
#include <iostream>
#include "Parse.h"
#include <string.h>
#include <sstream>
#include <stdlib.h> //strtof
#include <glm/gtc/epsilon.hpp> // epsilon equal
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
using namespace glm;

bool Box::epsilonEquals(float const a, float const b, float const epsilon)
{
    return std::abs(a - b) < epsilon;
}

vec3 Box::getNormal(const vec3 &point)
{
	if (epsilonEquals(point.x, min.x))
		return vec3(-1, 0, 0);
	else if (epsilonEquals(point.x, max.x))
		return vec3(1, 0, 0);
	else if (epsilonEquals(point.y, min.y))
		return vec3(0, -1, 0);
	else if (epsilonEquals(point.y, max.y))
		return vec3(0, 1, 0);
	else if (epsilonEquals(point.z, min.z))
		return vec3(0, 0, -1);
	else if (epsilonEquals(point.z, max.z))
		return vec3(0, 0, 1);
	else
	{
		std::cerr << "point did not intersect with the box" << std::endl;
		return vec3(0);
	}
}

vec3 Box::getCenter()
{
	return vec3(0.5)*(min + max);
}


Box *Box::parse(std::ifstream &infile, std::stringstream &s)
{
	std::string r, line;
	//std::cout << "parsing box" << std::endl;
	vec3 min = Parse::Vector(s);
	vec3 max = Parse::Vector(s);

	Box *box = new Box(min, max);
	Parse::Modifiers(box, infile);
	return box;
}

// return -1 if no intersection
// updates the ray's firstHit only if it is closer
int Box::getClosestIntersection(Ray &ray)
{
	vec3 rayPoint = ray.getPoint();
	vec3 rayDir = ray.getDir();
	float txmin, tymin, tzmin, txmax, tymax, tzmax, temp;
	txmin = tymin = tzmin = -1*std::numeric_limits<float>::infinity();
	txmax = tymax = tzmax = std::numeric_limits<float>::infinity();
	// Return false if ray is parallel to an axis and won't intersect
	if (epsilonEquals(rayDir.x, 0))
	{
		if (rayPoint.x < min.x || rayPoint.x > max.x)
			return -1;
	}
	else
	{
		txmin = (min.x - rayPoint.x)/rayDir.x;
		txmax = (max.x - rayPoint.x)/rayDir.x;
		if (txmin > txmax)
		{
			temp = txmin;
			txmin = txmax;
			txmax = temp;
		}
	}

	// y
	if (epsilonEquals(rayDir.y, 0))
	{
		if (rayPoint.y < min.y || rayPoint.y > max.y)
			return -1;
	}
	else
	{
		tymin = (min.y - rayPoint.y)/rayDir.y;
		tymax = (max.y - rayPoint.y)/rayDir.y;
		if (tymin > tymax)
		{
			temp = tymin;
			tymin = tymax;
			tymax = temp;
		}
	}

	// z
	if (epsilonEquals(rayDir.z, 0))
	{
		if (rayPoint.z < min.z || rayPoint.z > max.z)
			return -1;
	}
	else
	{
		tzmin = (min.z - rayPoint.z)/rayDir.z;
		tzmax = (max.z - rayPoint.z)/rayDir.z;
		if (tzmin > tzmax)
		{
			temp = tzmin;
			tzmin = tzmax;
			tzmax = temp;
		}
	}

	float smallestMax, largestMin;
	smallestMax = min(min(txmax, tymax), tzmax);
	largestMin = max(max(txmin, tymin), tzmin);

	// If max of mins is > min of maxes, then miss
	if (smallestMax < largestMin || smallestMax < 0)
		return -1;

	float solution;
	if (largestMin > 0)
		solution = largestMin;
	else
		solution = smallestMax;

    // Check if intersection is the closest
    if (ray.getIntersection().hasIntersection() == 0) // first one
    	ray.setIntersection(Intersection(1, solution, ray.getPoint() + solution*ray.getDir(), this));
    else if (solution < ray.getIntersection().getDistance()) // check if it is closer than the saved one
    	ray.setIntersection(Intersection(1, solution, ray.getPoint() + solution*ray.getDir(), this));
	return 0;
}

AABB *Box::calculateBBox()
{
	AABB *box = new AABB();
	box->reset(min);
	box->addPoint(max);
	return box;
}
