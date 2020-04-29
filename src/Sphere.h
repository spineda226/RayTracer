#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Shape.h"
#include "AABB.h"
#include "Ray.h"
#include "Intersection.h"
#include "Data.h"
#include <math.h>
#include <fstream>
#include <glm/glm.hpp>
using namespace glm;

class Sphere : public Shape
{
   public:
      Sphere(vec3 center, float radius) : Shape(), center(center), radius(radius) {}
      Sphere(vec3 center, float radius, vec3 color, properties *finish);
      ~Sphere() {};

      float getRadius() const { return radius; }

      vec3 getNormal(const glm::vec3 &point) override { return point - center; }
      vec3 getCenter() override { return center; }

      int getClosestIntersection(Ray &ray) override;
      AABB *calculateBBox() override;

      static Sphere *parse(std::ifstream &infile, std::stringstream &s);

   private:
      vec3 center;
      float radius;
};

float getSmallestT(float A, float B, float C);

#endif
