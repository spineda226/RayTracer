#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <fstream>
#include <glm/glm.hpp>
using namespace glm;


class Camera
{
   public:
      Camera(vec3 location=vec3(0), vec3 up=vec3(0,1,0), vec3 right=vec3(1.333,0,0), vec3 look_at=vec3(0,0,-1));
      vec3 getLoc() const { return location; }
      vec3 getUp() const { return up; }
      vec3 getRight() const { return right; }
      vec3 getLookAt() const { return look_at; }

      void setLoc(const vec3 loc) { location = loc; }
      void setUp(const vec3 u) { up = u; }
      void setRight(const vec3 r) { right = r; }
      void setLA(const vec3 la) { look_at = la; }

      static Camera *parse(std::ifstream &infile, std::stringstream &s);

   private:
      vec3 location;
      vec3 up;
      vec3 right;
      vec3 look_at;
};

#endif
