#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
using namespace glm;

class Camera
{
   public:
      Camera(vec3 location, vec3 up, vec3 right, vec3 look_at);
      vec3 getLoc() const { return location; }
      vec3 getUp() const { return up; }
      vec3 getRight() const { return right; }
      vec3 getLookAt() const { return look_at; }

   private:
      vec3 location;
      vec3 up;
      vec3 right;
      vec3 look_at;
};

#endif
