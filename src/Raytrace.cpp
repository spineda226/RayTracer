#include "Raytrace.h"
#include "Intersection.h"
#include "Image.h"
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#define EPSILON 0.001

// Raytrace all pixels
void raytrace(int g_width, int g_height, int i, int j,
  const vector<Shape *> &objects, const Camera &camera, const LightSource &light)
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
         printf("Pixel %d, %d\n", i, j);
         // Calculate the viewing ray for pixel i,j
         float U_s = -0.5 + (i+0.5)/g_width;
         float V_s = -0.5 + (j+0.5)/g_height;
         float W_s = -1; // near plane is one unit in front of the camera
         vec3 d = normalize(U_s*u + V_s*v + W_s*w); // d = P_w - C_0 (direction of the viewRay)

         Intersection firstHit;
         Ray viewRay(camera.getLoc(), d, firstHit);
         for (Shape *s : objects)
         {
            // update closest intersection
            s->getClosestIntersection(viewRay);
         }
         if (viewRay.getIntersection().hasIntersection() == 1) // If ray hits something in the scene find the color
         {
            vec3 color = 255.f*raycolor(objects, light, viewRay);
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
void single_raytrace(int g_width, int g_height, int i, int j,
  const vector<Shape *> &objects, const Camera &camera, const LightSource &light)
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
   Intersection firstHit;
   Ray viewRay(camera.getLoc(), d, firstHit);
   for (Shape *s : objects)
   {
      // update closest intersection
      s->getClosestIntersection(viewRay);
   }
   if (viewRay.getIntersection().hasIntersection() == 1)
   {
      vec3 color = 255.f*raycolor(objects, light, viewRay);
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

vec3 raycolor(const vector<Shape *> &objects, const LightSource &light, const Ray &r)
{
   Intersection firsthit = r.getIntersection();
   vec3 pt = firsthit.getPoint(); // get the point of intersection
   const properties *finish = firsthit.getObject()->getFinish(); // object properties
   vec3 color = (firsthit.getObject()->getColor())*(finish->ambient); // add ambient colors

   // Loop through lights
   bool in_shadow = false;
   vec3 l = normalize(light.getLoc() - pt); // light vector
   float light_pt_distance = distance(light.getLoc(), pt);
   Intersection firsthit_to_light;
   Ray lightRay(pt + l*vec3(EPSILON), l, firsthit_to_light); // ray to the light from intersection point
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
   if (in_shadow == false)
   {
      // Diffuse
      float N_dot_L = max(0, dot(normalize(firsthit.getObject()->getNormal(pt)), l));
      color += (finish->diffuse)*N_dot_L*light.getColor()*firsthit.getObject()->getColor();
      // Specular
      vec3 half_vector = normalize(normalize(vec3(-1)*r.getDir()) + l);
      float H_dot_N = max(0, dot(normalize(firsthit.getObject()->getNormal(pt)), half_vector));
      color += (finish->specular)*H_dot_N*light.getColor()*firsthit.getObject()->getColor();
   }
   return color;
}