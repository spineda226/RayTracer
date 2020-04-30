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

// TODO
bool AABB::hit(const Ray &r)
{
	return true;
}