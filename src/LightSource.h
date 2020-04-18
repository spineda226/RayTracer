#ifndef _LIGHTSRC_H_
#define _LIGHTSRC_H_

#include <glm/glm.hpp>

class LightSource
{
   public:
      LightSource(glm::vec3 location, glm::vec3 color);
      glm::vec3 getLoc() const { return location; }
      glm::vec3 getColor() const { return color; }

   private:
      glm::vec3 location;
      glm::vec3 color;
};

#endif
