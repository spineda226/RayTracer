#include "Sphere.h"
#include <iostream>

Sphere::Sphere(vec3 center, float radius, vec3 color, properties *finish) :
   center(center),
   radius(radius),
   color(color),
   finish(finish)
{
}

// Solve the quadratic formula and return the smallest positive solution
// Return -1 for non-positive solutions
// Precondition: determinant is positive
float getSmallestT(float A, float B, float C)
{
	float smallestT;
	float determinant = B*B - 4*A*C;
	float solution1 = (-B + sqrt(determinant))/(2*A);
	float solution2 = (-B - sqrt(determinant))/(2*A);
	if (solution1 < 0 && solution2 < 0)
		smallestT = -1;
	else if (solution1 >= 0 && solution2 >= 0)
		smallestT =  min(solution1, solution2);
	else
		smallestT = max(solution1, solution2);
    return smallestT;
}

// return -1 if no intersection
// updates the ray's firstHit only if it is closer
int Sphere::getClosestIntersection(Ray &ray)
{
	vec3 rayDir = ray.getDir();
	// Quadratic equation to test for intersection
	float A = dot(ray.getDir(), ray.getDir());
	float B = dot(2.f*ray.getDir(), ray.getPoint()-center);
	float C = dot(ray.getPoint()-center, ray.getPoint()-center) - radius*radius;
	float determinant = B*B - 4*A*C;
	if (determinant < 0) // no intersection
		return -1;
	float solution = getSmallestT(A, B, C); // distance from the ray origin
	if (solution < 0)
		return -1;
    // Check if intersection is the closest
    if (ray.getIntersection().hasIntersection() == 0) // first one
    	ray.setIntersection(Intersection(1, solution, ray.getPoint() + solution*ray.getDir(), this));
    else if (solution < ray.getIntersection().getDistance()) // check if it is closer than the saved one
    	ray.setIntersection(Intersection(1, solution, ray.getPoint() + solution*ray.getDir(), this));
	return 0;
}


