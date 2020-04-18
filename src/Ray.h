#ifndef _RAY_H_
#define _RAY_H_

#include "Intersection.h"

class Ray
{
   public:
      Ray() {};
      Ray(glm::vec3 point, glm::vec3 direction, Intersection intersection);
      ~Ray() {};

      glm::vec3 getPoint() const { return point; }
      glm::vec3 getDir() const { return direction; }
      Intersection getIntersection() const { return intersection; }
      void setIntersection(Intersection hit) { intersection = hit; }

   private:
      glm::vec3 point;
      glm::vec3 direction;
      Intersection intersection;
};

#endif
