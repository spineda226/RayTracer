#ifndef _INTERSECTION_H_
#define _INTERSECTION_H_

#include <glm/glm.hpp>
class Shape;

class Intersection
{
   public:
      Intersection() { exists = 0; };
      Intersection(int exists, float distance, glm::vec3 point, Shape *object);
      ~Intersection() {};
      int hasIntersection() const { return exists; }
      float getDistance() const { return distance; }
      glm::vec3 getPoint() const { return point; }
      Shape *getObject() const { return object; }

      void setPoint(glm::vec3 p_new) { point = p_new; }


   private:
   	int exists;
   	float distance;
      glm::vec3 point;
      Shape *object;
};

#endif
