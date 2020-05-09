#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Ray.h"
#include "AABB.h"
#include <glm/glm.hpp>
#include "Data.h"

class Shape
{
   public:
      Shape();
      Shape(vec3 color, properties *finish) : color(color), finish(finish) {}
      ~Shape() {};
      virtual int getClosestIntersection(Ray &ray) = 0;
      virtual glm::vec3 getNormal(const glm::vec3 &point) = 0;
      virtual glm::vec3 getCenter() = 0;
      virtual AABB *calculateBBox() = 0;

      vec3 getColor() const { return color; }
      void setColor(vec3 c) { color = c; }
      float getFilter() const { return filter; }
      void setFilter(float f) { filter = f; }
      properties *getFinish() const { return finish; }
      void setProperties(properties *f) { finish = f; }

   private:
      vec3 color;
      float filter;
      properties *finish;

};

#endif
