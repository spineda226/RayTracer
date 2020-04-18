#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Ray.h"
#include <glm/glm.hpp>
#include "Data.h"

class Shape
{
   public:
      Shape() {};
      ~Shape() {};
      virtual int getClosestIntersection(Ray &ray) = 0;
      virtual glm::vec3 getColor() = 0;
      virtual properties *getFinish() = 0;
      virtual glm::vec3 getNormal(const glm::vec3 &point) = 0;
};

#endif
