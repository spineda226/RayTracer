#include "Raytrace.h"
#include "Intersection.h"
#include "Image.h"
#include <iostream>
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#define EPSILON 0.001
using namespace std;

Ray& createReflect(const Ray &incident)
{
   vec3 d = incident.getDir();
   const Intersection& i = incident.getIntersection();
   vec3 n = normalize(i.getObject()->getNormal(i.getPoint()));
   vec3 reflected_dir = normalize(d - 2*dot(d, n)*n);
   Ray *reflectRay = new Ray(i.getPoint() + reflected_dir*vec3(EPSILON), reflected_dir);
   return *reflectRay;
}

// Raytrace all pixels with spatial data structure (BVH)
void raytrace(int g_width, int g_height,
  const BVH_Node &bvh, const std::vector<Shape *> &planes, const Camera &camera, const vector<LightSource *> &lights)
{
   //create an image
   auto image = make_shared<Image>(g_width, g_height);
   
   // Camera basis vectors
   vec3 u, v, w;
   u = camera.getRight(); // not normalized to account for aspect ratio 4:3
   v = camera.getUp(); // v is the up vector
   w = normalize(camera.getLoc() - camera.getLookAt()); // w is the opposite of where the camera is looking

   // Cast one ray per pixel
   // Line from the eye e to a point on the screen s: p(t) = e + t(s-e)
   // s = e + us * u + vs * v + ws * w
   for (int i = 0; i < g_width; ++i)
   {
      for (int j = 0; j < g_height; ++j)
      {
         //printf("Pixel %d, %d\n", i, j);
         // Calculate the viewing ray for pixel i,j
         float U_s = -0.5 + (i+0.5)/g_width;
         float V_s = -0.5 + (j+0.5)/g_height;
         float W_s = -1; // near plane is one unit in front of the camera
         vec3 d = normalize(U_s*u + V_s*v + W_s*w); // d = P_w - C_0 (direction of the viewRay)

         Ray viewRay(camera.getLoc(), d);
         bvh.getClosestIntersection(viewRay);
         for (Shape *s : planes)
         {
            // update closest intersection
            s->getClosestIntersection(viewRay);
         }
         if (viewRay.getIntersection().hasIntersection() == 1) // If ray hits something in the scene find the color
         {
            vec3 color = raycolor(bvh, planes, lights, viewRay);
            
            // Reflection
            vec3 object_n = normalize(viewRay.getIntersection().getObject()->getNormal(viewRay.getIntersection().getPoint()));
            //vec3 reflected_dir = normalize(d - 2*dot(d, object_n)*object_n);
            //Ray reflectRay(viewRay.getIntersection().getPoint() + reflected_dir*vec3(EPSILON), reflected_dir);

            Ray reflectRay = createReflect(viewRay);
            vec3 reflection_color = vec3(0);
            if (viewRay.getIntersection().getObject()->getFinish()->reflection != 0)
               reflection_color = recursive_raytrace(bvh, planes, lights, reflectRay, 1);

            // Refraction
            // n1 at start is 1.0 (starting in air)
            // n2 ior of material we are shading
            float n2 = viewRay.getIntersection().getObject()->getFinish()->ior;
            vec3 refracted_dir = ((1/n2)*(d-dot(d, object_n)*object_n));
            refracted_dir = normalize(refracted_dir - object_n*vec3((sqrt(1-pow((1/n2), 2)*(1-pow(dot(d, object_n), 2))))));
            Ray refractRay(viewRay.getIntersection().getPoint() + refracted_dir*vec3(EPSILON), refracted_dir);
            vec3 refraction_color = vec3(0);
            if (viewRay.getIntersection().getObject()->getFilter() != 0)
               refraction_color = recursive_raytrace(bvh, planes, lights, refractRay, 1, 1, true);
            
            //cout << "Color x: " << color.x << endl;
            float reflection_factor = (viewRay.getIntersection().getObject()->getFinish()->reflection);
            float refraction_factor = (viewRay.getIntersection().getObject()->getFilter());

            vec3 total_color = (1-refraction_factor)*(1-reflection_factor)*color + 
                                 reflection_factor*reflection_color + refraction_factor*refraction_color;
            total_color = 255.f*total_color;
            //cout << total_color.x << endl;
            unsigned int red = min(255, (unsigned int)std::round(total_color.x));
            unsigned int green = min(255, (unsigned int)std::round(total_color.y));
            unsigned int blue = min(255, (unsigned int)std::round(total_color.z));
            
            image->setPixel(i, j , red, green, blue);
         } 
         else // else color it background color
            image->setPixel(i, j, 0, 0, 0);
      }
   }
   //write out the image
   image->writeToFile("out.png");
}

// Recursively raytrace a single ray and return the generated color
vec3 recursive_raytrace(const BVH_Node &bvh, const std::vector<Shape *> &planes, const std::vector<LightSource *> &lights,
                        Ray &ray, int iteration, float n1, bool enter)
{
   if (iteration == 6)
      return vec3(0);

   bvh.getClosestIntersection(ray);
   for (Shape *s : planes)
      s->getClosestIntersection(ray);
   if (ray.getIntersection().hasIntersection() == 1) // If ray hits something in the scene find the color
   {
      vec3 color = raycolor(bvh, planes, lights, ray);

      vec3 d = ray.getDir();
      vec3 object_n = normalize(ray.getIntersection().getObject()->getNormal(ray.getIntersection().getPoint()));
      //vec3 reflected_dir = normalize(d - 2*dot(d, object_n)*object_n);
      //Ray reflectRay(ray.getIntersection().getPoint() + reflected_dir*vec3(EPSILON), reflected_dir);
      Ray reflectRay = createReflect(ray);
      vec3 reflection_color = vec3(0);
      if (ray.getIntersection().getObject()->getFinish()->reflection != 0)
         reflection_color = recursive_raytrace(bvh, planes, lights, reflectRay, iteration+1);

      // Refraction
      // n1 at start is 1.0 (starting in air)
      // n2 ior of material we are shading
      float n2;
      if (enter) // now exiting 
      {
         object_n = -object_n;
         n2 = n1;
         n1 = ray.getIntersection().getObject()->getFinish()->ior;
      }
      else
      {
         n2 = ray.getIntersection().getObject()->getFinish()->ior;
         enter = true;
      }

      vec3 refracted_dir = ((n1/n2)*(d-dot(d, object_n)*object_n));
      refracted_dir = normalize(refracted_dir - object_n*vec3((sqrt(1-pow((n1/n2), 2)*(1-pow(dot(d, object_n), 2))))));
      Ray refractRay(ray.getIntersection().getPoint() + refracted_dir*vec3(EPSILON), refracted_dir);
      vec3 refraction_color = vec3(0);
      if ((ray.getIntersection().getObject()->getFilter()) != 0)
         refraction_color = recursive_raytrace(bvh, planes, lights, refractRay, iteration+1, n2, enter);

      float reflection_factor = (ray.getIntersection().getObject()->getFinish()->reflection);
      float refraction_factor = (ray.getIntersection().getObject()->getFilter());
      vec3 total_color = (1-refraction_factor)*(1-reflection_factor)*color + 
                                 reflection_factor*reflection_color + refraction_factor*refraction_color;

      return total_color;
   } 
   else // else color it background color
      return vec3(0);
}

// Cast a single ray for pixel x,y and print out the results
void single_raytrace(int g_width, int g_height, int i, int j,
  const BVH_Node &bvh, const std::vector<Shape *> &planes, const Camera &camera, const vector <LightSource *> &lights)
{
   vec3 u, v, w;
   u = camera.getRight(); // not normalized to account for aspect ratio 4:3
   v = camera.getUp(); // v is the up vector
   w = normalize(camera.getLoc() - camera.getLookAt()); // w is the opposite of where the camera is looking
   float U_s = -0.5 + (i+0.5)/g_width;
   float V_s = -0.5 + (j+0.5)/g_height;
   float W_s = -1; // near plane is one unit in front of the camera
   vec3 d = normalize(U_s*u + V_s*v + W_s*w); // d = P_w - C_0
   printf("Pixel: [%d, %d] Ray: {%f %f %f} -> {%f %f %f}\n",
           i, j, camera.getLoc().x, camera.getLoc().y, camera.getLoc().z, d.x, d.y, d.z);
   Ray viewRay(camera.getLoc(), d);
   bvh.getClosestIntersection(viewRay);
   for (Shape *s : planes)
      s->getClosestIntersection(viewRay);

   if (viewRay.getIntersection().hasIntersection() == 1)
   {
      vec3 color = 255.f*raycolor(bvh, planes, lights, viewRay);
      unsigned int red = (unsigned int)std::round(color.x);
      unsigned int blue = (unsigned int)std::round(color.y);
      unsigned int green = (unsigned int)std::round(color.z);
      printf("T = %f\n", viewRay.getIntersection().getDistance());
      printf("Color: %d %d %d\n", red, blue, green);
   }
   else
   {
      printf("No hit\n");
   }
}

vec3 raycolor(const BVH_Node &bvh, const std::vector<Shape *> &planes, const vector<LightSource *> &lights, const Ray &r)
{
   Intersection firsthit = r.getIntersection();
   vec3 pt = firsthit.getPoint(); // get the point of intersection
   const properties *finish = firsthit.getObject()->getFinish(); // object properties
   vec3 color = (firsthit.getObject()->getColor())*(finish->ambient); // add ambient colors

   // Loop through lights
   for (LightSource *light : lights)
   {
      bool in_shadow = false;
      vec3 l = normalize(light->getLoc() - pt); // light vector
      float light_pt_distance = distance(light->getLoc(), pt);
      //Intersection firsthit_to_light;
      Ray lightRay(pt + l*vec3(EPSILON), l); // ray to the light from intersection point
      for (Shape *s : planes) // check if you hit anything on the way to the light
      {
         s->getClosestIntersection(lightRay);
         if (lightRay.getIntersection().hasIntersection() == 1)
         {
            if (lightRay.getIntersection().getDistance() < light_pt_distance)
            {
               in_shadow = true;
               break;
            }
         }
      }
      if (in_shadow == false)
      {
         if (bvh.hit(lightRay))
         {
            if (lightRay.getIntersection().getDistance() < light_pt_distance)
               in_shadow = true;
         }
      }
      if (in_shadow == false)
      {
         // Diffuse
         float N_dot_L = max(0, dot(normalize(firsthit.getObject()->getNormal(pt)), l));
         color += (finish->diffuse)*N_dot_L*light->getColor()*firsthit.getObject()->getColor();
         // Specular
         vec3 half_vector = normalize(normalize(vec3(-1)*r.getDir()) + l);
         float H_dot_N = max(0, dot(normalize(firsthit.getObject()->getNormal(pt)), half_vector));
         color += (finish->specular)*H_dot_N*light->getColor()*firsthit.getObject()->getColor();
      }
   }
   return color;
}


/************************************** WITHOUT SPATIAL DATA STRUCTURE **********************************************************/
// Raytrace all pixels
void raytrace_without_spatial(int g_width, int g_height, int i, int j,
  const vector<Shape *> &objects, const std::vector<Shape *> &planes, const Camera &camera, const vector<LightSource *> &lights)
{
   //create an image
   auto image = make_shared<Image>(g_width, g_height);
   
   // Camera basis vectors
   vec3 u, v, w;
   u = camera.getRight(); // not normalized to account for aspect ratio 4:3
   v = camera.getUp(); // v is the up vector
   w = normalize(camera.getLoc() - camera.getLookAt()); // w is the opposite of where the camera is looking

   // Cast one ray per pixel
   // Line from the eye e to a point on the screen s: p(t) = e + t(s-e)
   // s = e + us * u + vs * v + ws * w
   for (int i = 0; i < g_width; ++i)
   {
      for (int j = 0; j < g_height; ++j)
      {
         //printf("Pixel %d, %d\n", i, j);
         // Calculate the viewing ray for pixel i,j
         float U_s = -0.5 + (i+0.5)/g_width;
         float V_s = -0.5 + (j+0.5)/g_height;
         float W_s = -1; // near plane is one unit in front of the camera
         vec3 d = normalize(U_s*u + V_s*v + W_s*w); // d = P_w - C_0 (direction of the viewRay)

         //Intersection firstHit;
         Ray viewRay(camera.getLoc(), d);
         for (Shape *s : objects)
            s->getClosestIntersection(viewRay);
         for (Shape *s : planes)
            s->getClosestIntersection(viewRay);

         if (viewRay.getIntersection().hasIntersection() == 1) // If ray hits something in the scene find the color
         {
            vec3 color = 255.f*raycolor_without_spatial(objects, planes, lights, viewRay);
            unsigned int red = min(255, (unsigned int)std::round(color.x));
            unsigned int blue = min(255, (unsigned int)std::round(color.y));
            unsigned int green = min(255, (unsigned int)std::round(color.z));
            image->setPixel(i, j , red, blue, green);
         } 
         else // else color it background color
            image->setPixel(i, j, 0, 0, 0);
      }
   }
   //write out the image
   image->writeToFile("out.png");
}

// Cast a single ray for pixel x,y and print out the results
void single_raytrace_without_spatial(int g_width, int g_height, int i, int j,
  const vector<Shape *> &objects, const std::vector<Shape *> &planes, const Camera &camera, const vector <LightSource *> &lights)
{
   vec3 u, v, w;
   u = camera.getRight(); // not normalized to account for aspect ratio 4:3
   v = camera.getUp(); // v is the up vector
   w = normalize(camera.getLoc() - camera.getLookAt()); // w is the opposite of where the camera is looking
   float U_s = -0.5 + (i+0.5)/g_width;
   float V_s = -0.5 + (j+0.5)/g_height;
   float W_s = -1; // near plane is one unit in front of the camera
   vec3 d = normalize(U_s*u + V_s*v + W_s*w); // d = P_w - C_0
   printf("Pixel: [%d, %d] Ray: {%f %f %f} -> {%f %f %f}\n",
           i, j, camera.getLoc().x, camera.getLoc().y, camera.getLoc().z, d.x, d.y, d.z);
   //Intersection firstHit;
   Ray viewRay(camera.getLoc(), d);
   for (Shape *s : objects)
      s->getClosestIntersection(viewRay);
   for (Shape *s : planes)
      s->getClosestIntersection(viewRay);

   if (viewRay.getIntersection().hasIntersection() == 1)
   {
      vec3 color = 255.f*raycolor_without_spatial(objects, planes, lights, viewRay);
      unsigned int red = (unsigned int)std::round(color.x);
      unsigned int blue = (unsigned int)std::round(color.y);
      unsigned int green = (unsigned int)std::round(color.z);
      printf("T = %f\n", viewRay.getIntersection().getDistance());
      printf("Color: %d %d %d\n", red, blue, green);
   }
   else
   {
      printf("No hit\n");
   }
}

vec3 raycolor_without_spatial(const vector<Shape *> &objects, const std::vector<Shape *> &planes, const vector<LightSource *> &lights, const Ray &r)
{
   Intersection firsthit = r.getIntersection();
   vec3 pt = firsthit.getPoint(); // get the point of intersection
   const properties *finish = firsthit.getObject()->getFinish(); // object properties
   vec3 color = (firsthit.getObject()->getColor())*(finish->ambient); // add ambient colors

   // Loop through lights
   for (LightSource *light : lights)
   {
      bool in_shadow = false;
      vec3 l = normalize(light->getLoc() - pt); // light vector
      float light_pt_distance = distance(light->getLoc(), pt);
      //Intersection firsthit_to_light;
      Ray lightRay(pt + l*vec3(EPSILON), l); // ray to the light from intersection point
      for (Shape *s : objects) // check if you hit anything on the way to the light
      {
         s->getClosestIntersection(lightRay);
         if (lightRay.getIntersection().hasIntersection() == 1)
         {
            if (lightRay.getIntersection().getDistance() < light_pt_distance)
            {
               in_shadow = true;
               break;
            }
         }
      }
      
      if  (in_shadow == false) // check if a plane was in the way
      {
         for (Shape *s : planes)
         {
            s->getClosestIntersection(lightRay);
            if (lightRay.getIntersection().hasIntersection() == 1)
            {
               if (lightRay.getIntersection().getDistance() < light_pt_distance)
               {
                  in_shadow = true;
                  break;
               }
            }
         }
      }
      
      if (in_shadow == false)
      {
         // Diffuse
         float N_dot_L = max(0, dot(normalize(firsthit.getObject()->getNormal(pt)), l));
         color += (finish->diffuse)*N_dot_L*light->getColor()*firsthit.getObject()->getColor();
         // Specular
         vec3 half_vector = normalize(normalize(vec3(-1)*r.getDir()) + l);
         float H_dot_N = max(0, dot(normalize(firsthit.getObject()->getNormal(pt)), half_vector));
         color += (finish->specular)*H_dot_N*light->getColor()*firsthit.getObject()->getColor();
      }
   }
   return color;
}