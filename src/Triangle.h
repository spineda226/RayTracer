#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "Shape.h"
#include "Ray.h"
#include "Intersection.h"
#include "AABB.h"
#include "Data.h"
#include <math.h>
#include <glm/glm.hpp>
using namespace glm;

class Triangle : public Shape
{
   public:
      Triangle(vec3 p1, vec3 p2, vec3 p3) : Shape(), p1(p1), p2(p2), p3(p3) {}
      Triangle(vec3 p1, vec3 p2, vec3 p3, vec3 color, properties *finish);
      ~Triangle() {};

      vec3 getP1() const { return p1; }
      vec3 getP2() const { return p2; }
      vec3 getP3() const { return p3; }
      const vec3 getMins() const;
      const vec3 getMaxs() const;

      float computeGamma(vec3 &p0, vec3 &d, float detA);
      float computeBeta(vec3 &p0, vec3 &d, float detA);
      float computeT(vec3 &p0, vec3 &d, float detA);

      vec3 getNormal(const glm::vec3 &point) override { return cross(p2-p1, p3-p1); }
      vec3 getCenter() override { return (p1 + p2 + p3)/glm::vec3(3.0); }

      int getClosestIntersection(Ray &ray) override;
      AABB *calculateBBox() override;

      static Triangle *parse(std::ifstream &infile, std::stringstream &s);
      bool triangleAABBIntersect(vec3& p, vec3& deltaP) const;

   private:
      vec3 p1, p2, p3;
};

float calcualteDeterminant(vec3 &row1, vec3 &row2, vec3 &row3);
#endif
