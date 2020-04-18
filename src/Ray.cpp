#include "Ray.h"

#include <glm/glm.hpp>

Ray::Ray(glm::vec3 point, glm::vec3 direction, Intersection intersection):
   point(point), direction(direction), intersection(intersection)
{
}
