#ifndef _BOX_H_
#define _BOX_H_

#include "Shape.h"
#include "AABB.h"
#include "Ray.h"
#include "Intersection.h"
#include "Data.h"
#include <math.h>
#include <fstream>
#include <glm/glm.hpp>

class Box : public Shape
{
   public:
      Box(glm::vec3 min, glm::vec3 max) : Shape(), min(min), max(max) {}
      Box(glm::vec3 min, glm::vec3 max, glm::vec3 color, properties *finish) : Shape(color, finish), min(min), max(max) {}
      ~Box() {};

      glm::vec3 getMin() const { return min; }
      glm::vec3 getMax() const { return max; }

      glm::vec3 getNormal(const glm::vec3 &point) override;
      glm::vec3 getCenter() override;
      int getClosestIntersection(Ray &ray) override;
      AABB *calculateBBox() override;

      static Box *parse(std::ifstream &infile, std::stringstream &s);
      static bool epsilonEquals(float const a, float const b, float const epsilon = 0.0001f);

   private:
      glm::vec3 min;
      glm::vec3 max;
};

#endif
