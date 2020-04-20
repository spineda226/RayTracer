#include "Data.h"
#include "Sphere.h"
#include "Plane.h"

void init_sphere_pov(std::vector<Shape *> *objects, Camera **camera, std::vector<LightSource *> *lights)
{
   *camera = new Camera(vec3(0, 0, 14), vec3(0, 1, 0), vec3(1.33333, 0, 0), vec3(0, 0, 0));
   LightSource *light = new LightSource(vec3(-100, 100, 100), vec3(1.5, 1.5, 1.5));
   lights->push_back(light);

   properties *little_props = new properties{0.2, 0.4, 0, 0, 0, 0, 0};
   Sphere *little = new Sphere(vec3(0, 0, 0), 1.01, vec3(1, 1, 1), little_props);

   properties *medium_props = new properties{0.2, 0.4, 0, 0, 0, 0, 0};
   Sphere *medium = new Sphere(vec3(0, 0, -1.5), 2.25, vec3(0, 0, 0), medium_props);

   properties *big_props = new properties{0.2, 0.4, 0, 0, 0, 0, 0};
   Sphere *big = new Sphere(vec3(0, 0, -4.5), 3.75, vec3(0.25, 0.25, 0.25), big_props);

   properties *left_props = new properties{0.2, 0.4, 0, 0, 0, 0, 0};
   Sphere *left = new Sphere(vec3(-5.5, 3, 0), 1.1, vec3(0.12, 0.34, 0.56), left_props);

   objects->push_back(little);
   objects->push_back(medium);
   objects->push_back(big);
   objects->push_back(left);
}

void init_simple_pov(std::vector<Shape *> *objects, Camera **camera, std::vector<LightSource *> *lights)
{
  *camera = new Camera(vec3(0, 0, 14), vec3(0, 1, 0), vec3(1.33333, 0, 0), vec3(0, 0, 0));
  LightSource *light = new LightSource(vec3(-100, 100, 100), vec3(1.5, 1.5, 1.5));
  lights->push_back(light);

  properties *simple_props = new properties{0.2, 0.4, 0, 0, 0, 0, 0};
	Sphere *simple = new Sphere(vec3(0, 0, 0), 2, vec3(1, 0, 1), simple_props);

  properties *plane_props = new properties{0.4, 0.8, 0, 0, 0, 0, 0};
  Plane *simple_plane = new Plane(vec3(0, 1, 0), -4, vec3(0.2, 0.2, 0.8), plane_props);

  objects->push_back(simple);
  objects->push_back(simple_plane);
}

void init_simple_cam_pov(std::vector<Shape *> *objects, Camera **camera, std::vector<LightSource *> *lights, int option)
{
  LightSource *light;
  if (option == 0) // Look down -z axis
  {
    *camera = new Camera(vec3(0, 0, 14), vec3(0, 1, 0), vec3(1.33333, 0, 0), vec3(0, 0, 0));
    light = new LightSource(vec3(-100, 100, 100), vec3(1.5, 1.5, 1.5));
  }
  else if (option == 1) // Look down +x axis
  {
    *camera = new Camera(vec3(-14, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1.33333), vec3(0, 0, 0));
    light = new LightSource(vec3(-100, 100, 100), vec3(1.5, 1.5, 1.5));
  }
  else if (option == 2) // Look down -x axis
  {
    *camera = new Camera(vec3(14, 0, 0), vec3(0, 1, 0), vec3(0, 0, -1.33333), vec3(0, 0, 0));
    light = new LightSource(vec3(-100, 100, 100), vec3(1.5, 1.5, 1.5));
  }
  else if (option == 3)
  {
    *camera = new Camera(vec3(7, 0, 7), vec3(0, 1, 0), vec3(0.94, 0, -0.94), vec3(0, 0, 0));
    light = new LightSource(vec3(-100, 100, 100), vec3(1.5, 1.5, 1.5));
  }
  lights->push_back(light);

  properties *s1_props = new properties{0.2, 0.4, 0.1, 0.1, 0, 0, 0};
  Sphere *s1 = new Sphere(vec3(-3.5, 0, 0), 1, vec3(1, 0, 1), s1_props);

  properties *s2_props = new properties{0.2, 0.4, 0.1, 0.1, 0, 0, 0};
  Sphere *s2  = new Sphere(vec3(1, 0, 0), 1.5, vec3(0, 1, 1), s2_props);

  properties *s3_props = new properties{0.2, 0.4, 0.1, 0.1, 0, 0, 0};
  Sphere *s3 = new Sphere(vec3(-2.5, 0, -4), 2, vec3(1, 1, 0), s3_props);

  properties *s4_props = new properties{0.2, 0.4, 0.1, 0.1, 0, 0, 0};
  Sphere *s4 = new Sphere(vec3(2.8, 0, -4), 2.5, vec3(0.1, 1, 0.5), s4_props);

  properties *s5_props = new properties{0.2, 0.4, 0.1, 0.1, 0, 0, 0};
  Sphere *s5 = new Sphere(vec3(0, 4, 0), 1.1, vec3(0.1, 0.1, 1), s5_props);

  properties *plane_props = new properties{0.4, 0.8, 0, 0, 0, 0, 0};
  Plane *simple_plane = new Plane(vec3(0, 1, 0), -4, vec3(0.2, 0.6, 0.8), plane_props);

  objects->push_back(s1);
  objects->push_back(s2);
  objects->push_back(s3);
  objects->push_back(s4);
  objects->push_back(s5);
  objects->push_back(simple_plane);
}


