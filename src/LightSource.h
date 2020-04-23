#ifndef _LIGHTSRC_H_
#define _LIGHTSRC_H_

#include <fstream>
#include <glm/glm.hpp>

class LightSource
{
   public:
      LightSource(glm::vec3 location, glm::vec3 color);
      glm::vec3 getLoc() const { return location; }
      glm::vec3 getColor() const { return color; }

      void setLoc(const glm::vec3 loc) { location = loc; }
      void setColor(const glm::vec3 c) { color = c; }

      static LightSource *parse(std::ifstream &infile, std::stringstream &s);

   private:
      glm::vec3 location;
      glm::vec3 color;
};

#endif
