#include "Intersection.h"

Intersection::Intersection(int exists, float distance, glm::vec3 point, Shape *object):
   exists(exists), distance(distance), point(point), object(object)
{
}
