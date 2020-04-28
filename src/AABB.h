#ifndef _AABB_H_
#define _AABB_H_

#include <glm/glm.hpp>
using namespace glm;

class AABB
{
   public:
   	  AABB() {}
      AABB(vec3 min, vec3 max) : min(min), max(max) {}
      ~AABB() {};

      vec3 getMin() const { return min; }
      vec3 getMax() const { return max; }

      void reset(vec3 pt);
      void addPoint(vec3 pt);
      void addBox(AABB other);

   private:
      vec3 min, max;
};

#endif