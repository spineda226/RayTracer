#include "AABB.h"

// initializes the min and max to pt
void AABB::reset(vec3 pt)
{
	min = max = pt;
}

// recalculates the bounds of the AABB given a new point
void AABB::addPoint(vec3 pt)
{
	min.x = glm::min(min.x, pt.x);
	min.y = glm::min(min.y, pt.y);
	min.z = glm::min(min.z, pt.z);
	max.x = glm::max(max.x, pt.x);
	max.y = glm::max(max.y, pt.y);
	max.z = glm::max(max.z, pt.z);
}

// combines two AABB into a bigger one
void AABB::addBox(AABB other)
{
	addPoint(other.min);
	addPoint(other.max);
}

bool AABB::epsilonEquals(float const a, float const b, float const epsilon)
{
    return std::abs(a - b) < epsilon;
}

// returns true or false if the ray hits the box
// does not update the ray's intersection point
bool AABB::hit(const Ray &r)
{
	vec3 rayPoint = r.getPoint();
	vec3 rayDir = r.getDir();
	float txmin, tymin, tzmin, txmax, tymax, tzmax, temp;
	txmin = tymin = tzmin = -1*std::numeric_limits<float>::infinity();
	txmax = tymax = tzmax = std::numeric_limits<float>::infinity();
	// Return false if ray is parallel to an axis and won't intersect
	if (epsilonEquals(rayDir.x, 0))
	{
		if (rayPoint.x < min.x || rayPoint.x > max.x)
			return false;
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
			return false;
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
			return false;
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
	smallestMax = glm::min(glm::min(txmax, tymax), tzmax);
	largestMin = glm::max(glm::max(txmin, tymin), tzmin);

	// If max of mins is > min of maxes, then miss
	if (smallestMax < largestMin || smallestMax < 0)
		return false;
	return true;
}

// Taken from Brent Williams
// Turns bounding box into a square
void AABB::square()
{
   float xLen = max.x - min.x;
   float yLen = max.y - min.y;
   float zLen = max.z - min.z;
   float largest = xLen;
   
   if (yLen > largest)
      largest = yLen;
   if (zLen > largest)
      largest = zLen;

   float halfLength = xLen * 0.5f; // should this be largest?

   float centerX = (min.x + max.x) * 0.5f;
   min.x = centerX - halfLength;
   max.x = centerX + halfLength;

   float centerY = (min.y + max.y) * 0.5f;
   min.y = centerY - halfLength;
   max.y = centerY + halfLength;

   float centerZ = (min.z + max.z) * 0.5f;
   min.z = centerZ - halfLength;
   max.z = centerZ + halfLength;
}