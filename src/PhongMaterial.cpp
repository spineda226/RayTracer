/**
 * PhongMaterial.cpp
 * 
 * @author Brent Williams brent.robert.williams@gmail.com
 */ 

#include "PhongMaterial.h"

glm::vec3 PhongMaterial::calculateSurfaceColor(Ray ray, glm::vec3 hitPosition, glm::vec3 n)
{  
   glm::vec3 finalColor = glm::vec3(0.0f,0.0f,0.0f);
   glm::vec3 lc(1.0,1.0,1.0); // Light color
   
   // // toyStore lights
   // float intensity = 0.18f;
   // glm::vec3 lightPositions[8];
   // // Top Floor
   // lightPositions[0] = glm::vec3( 12,  12,  12);
   // lightPositions[1] = glm::vec3( 12,  12, -12);
   // lightPositions[2] = glm::vec3(-12,  12,  12);
   // lightPositions[3] = glm::vec3(-12,  12, -12);

   // // Bottom Floor
   // lightPositions[0] = glm::vec3( 12,  -0.5,  12);
   // lightPositions[1] = glm::vec3( 12,  -0.5, -12);
   // lightPositions[2] = glm::vec3(-12,  -0.5,  12);
   // lightPositions[3] = glm::vec3(-12,  -0.5, -12);

   // cornellBox, bunny and buddha lights
   // float intensity = 0.35f;
   // glm::vec3 lightPositions[1];
   // lightPositions[0] = glm::vec3( 100,  1000,  100);
   // lightPositions[1] = glm::vec3( 100,  1000, -100);
   // lightPositions[2] = glm::vec3(-100,  1000,  100);
   // lightPositions[3] = glm::vec3(-100,  1000, -100);
   // lightPositions[4] = glm::vec3(0,  10, 0);
   float intensity = 1.f;
   glm::vec3 lightPositions[1];
   lightPositions[0] = glm::vec3(0,  10, 0);

   // Ambient
   lc = lc * intensity;
   glm::vec3 ambientComponent = ka * lc;
   finalColor += ambientComponent;
   for (int i = 0; i < 1; ++i)
   {
      glm::vec3 lightPosition = lightPositions[i];
      glm::vec3 l = glm::normalize(lightPosition - hitPosition);
      glm::vec3 v = normalize(ray.getDir());

      // Diffuse
      float nDotL = glm::max(glm::dot(n, l), 0.0f);
      glm::vec3 diffuseComponent = kd * nDotL * lc;

      // Specular
      glm::vec3 r = glm::reflect(l, n);
      float vDotR = glm::max(glm::dot(v,r), 0.0f);
      glm::vec3 specularComponent = ks * pow(vDotR, ns) * lc;

      finalColor += diffuseComponent + specularComponent;
   }

   return finalColor;
}