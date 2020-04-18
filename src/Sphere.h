#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Shape.h"
#include "Ray.h"
#include "Intersection.h"
#include "Data.h"
#include <math.h>
#include <glm/glm.hpp>
using namespace glm;

class Sphere : public Shape
{
   public:
      Sphere(vec3 center, float radius, vec3 color, properties *finish);
      ~Sphere() {};

      vec3 getCenter() const { return center; }
      float getRadius() const { return radius; }

      vec3 getColor() override { return color; }
      vec3 getNormal(const glm::vec3 &point) override { return point - center; }
      properties *getFinish() override { return finish; }

      int getClosestIntersection(Ray &ray) override;

   private:
      vec3 center;
      float radius;
      vec3 color;
      properties *finish;
};

float getSmallestT(float A, float B, float C);

#endif
