#ifndef _PLANE_H_
#define _PLANE_H_

#include "Shape.h"
#include "Ray.h"
#include "Data.h"
#include <math.h>
#include <glm/glm.hpp>
using namespace glm;

class Plane : public Shape
{
   public:
      Plane(vec3 normal, float distance, vec3 color, properties *finish);
      ~Plane() {};
      vec3 getNormal() const { return normal; }
      float getDistance() const { return distance; }
      vec3 getColor() override { return color; }
      vec3 getNormal(const glm::vec3 &point) override { return normal; }
      properties *getFinish() override { return finish; }

      int getClosestIntersection(Ray &ray) override;

   private:
      vec3 normal;
      float distance; // how far from the origin the plane is along the normal
      vec3 color;
      properties *finish;
};

#endif
