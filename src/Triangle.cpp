#include "Triangle.h"
#include <iostream>

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3, vec3 color, properties *finish) :
   p1(p1), p2(p2), p3(p3), Shape(color, finish)
{
}

// return -1 if no intersection
// updates the ray's firstHit only if it is closer
int Triangle::getClosestIntersection(Ray &ray)
{
	vec3 rayPoint = ray.getPoint();
	vec3 rayDir = ray.getDir();

	// Construct 'A' matrix
	vec3 row1(vec3(p1.x-p2.x, p1.x-p3.x, rayDir.x)); // x_a - x_b, x_a - x_c, x_d
	vec3 row2(vec3(p1.y-p2.y, p1.y-p3.y, rayDir.y)); // y_a - y_b, y_a - y_c, y_d
	vec3 row3(vec3(p1.z-p2.z, p1.z-p3.z, rayDir.z)); // z_a - z_b, z_a - z_c, z_d
	float detA = calcualteDeterminant(row1, row2, row3);
	if (detA == 0) // no intersection
		return -1;
	float t = computeT(rayPoint, rayDir, detA);
	// Stop if t is negative or there is already a closer intersection
	if (t < 0 || (ray.getIntersection().hasIntersection() && (t > ray.getIntersection().getDistance())))
		return -1;
	float gamma = computeGamma(rayPoint, rayDir, detA);
	if ((gamma < 0) || gamma > 1)
		return -1;
	float beta = computeBeta(rayPoint, rayDir, detA);
	if ((beta < 0) || beta > 1)
		return -1;
	float alpha = 1 - gamma - beta;
	if ((alpha < 0) || alpha > 1)
		return -1;

	// t is the closest intersection so save it 
	ray.setIntersection(Intersection(1, t, ray.getPoint() + t*rayDir, this));
	return 0;
}

// compute t (intersection distance if it exists) 
float Triangle::computeT(vec3 &p0, vec3 &d, float detA)
{
	vec3 row1(p1.x-p2.x, p1.x-p3.x, p1.x-p0.x);
	vec3 row2(p1.y-p2.y, p1.y-p3.y, p1.y-p0.y);
	vec3 row3(p1.z-p2.z, p1.z-p3.z, p1.z-p0.z);
	return calcualteDeterminant(row1, row2, row3)/detA;
}

// compute the gamma (barycentric coordinate) for the intersection point (if it exists)
float Triangle::computeGamma(vec3 &p0, vec3 &d, float detA)
{
	vec3 row1(p1.x-p2.x, p1.x-p0.x, d.x);
	vec3 row2(p1.y-p2.y, p1.y-p0.y, d.y);
	vec3 row3(p1.z-p2.z, p1.z-p0.z, d.z);
	return calcualteDeterminant(row1, row2, row3)/detA;
}

// compute the beta (barycentric coordinate) for the intersection point (if it exists)
float Triangle::computeBeta(vec3 &p0, vec3 &d, float detA)
{
	vec3 row1(p1.x-p0.x, p1.x-p3.x, d.x);
	vec3 row2(p1.y-p0.y, p1.y-p3.y, d.y);
	vec3 row3(p1.z-p0.z, p1.z-p3.z, d.z);
	return calcualteDeterminant(row1, row2, row3)/detA;
}

// calculates the determinant of a 3x3 matrix
float calcualteDeterminant(vec3 &row1, vec3 &row2, vec3 &row3)
{
	float first = row1.x*row2.y*row3.z - row1.x*row2.z*row3.y;  // aei - afh
	float second = -row1.y*row2.x*row3.z + row1.y*row2.z*row3.x;  // -bdi + bfg
	float third = row1.z*row2.x*row3.y - row1.z*row2.y*row3.x;  // cdh - ceg
	return first + second + third;
}
