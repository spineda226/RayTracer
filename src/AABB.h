#ifndef _AABB_H_
#define _AABB_H_

#include <glm/glm.hpp>
#include "Ray.h"
using namespace glm;
#include <fstream> // ifstream, getline

class AABB
{
   public:
   	AABB() {}
      AABB(vec3 min, vec3 max) : min(min), max(max) {}
      ~AABB() {};

      vec3 getMin() const { return min; }
      vec3 getMax() const { return max; }
      vec3 getCenter() const { return (min + max) * 0.5f; }

      void reset(vec3 pt);
      void addPoint(vec3 pt);
      void addBox(AABB other);
      void square(); // look into this (why is square based on xlen)

      bool hit(const Ray &r);
      static bool epsilonEquals(float const a, float const b, float const epsilon = 0.0001f);

      // friend: function defined outside the class but has access to private members
      friend std::ostream& operator<<(std::ostream& out, const AABB &bb) {
         out << "Min: <" << bb.min.x << " " << bb.min.y << " " << bb.min.z << ">, Max: <" <<
                bb.max.x << " " << bb.max.y << " " << bb.max.z << ">" << std::endl;
         return out;
      }

   private:
      vec3 min, max;
};

#endif