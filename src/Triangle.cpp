#include "Triangle.h"
#include <iostream>
#include "Parse.h"
#include <string.h>
#include <sstream>
#include <stdlib.h> //strtof

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3, vec3 color, properties *finish) :
   p1(p1), p2(p2), p3(p3), Shape(color, finish)
{
}

Triangle *Triangle::parse(std::ifstream &infile, std::stringstream &s)
{
	//std::cout << "parsing triangle" << std::endl;
	std::string d, line;
	// p1
	s.clear();
	getline(infile, line);
	s.str(line);
	vec3 p1 = Parse::Vector(s);

	// p2
	s.clear();
	getline(infile, line);
	s.str(line);
	vec3 p2 = Parse::Vector(s);

	// p3
	s.clear();
	getline(infile, line);
	s.str(line);
	vec3 p3 = Parse::Vector(s);

	Triangle *triangle = new Triangle(p1, p2, p3);
	Parse::Modifiers(triangle, infile);
	return triangle;
}

// return -1 if no intersection
// updates the ray's firstHit only if it is closer
int Triangle::getClosestIntersection(Ray &ray)
{
	vec3 rayPoint = ray.getPoint();
	vec3 rayDir = ray.getDir();

	// Construct 'A' matrix
	vec3 row1(vec3(p1.x-p2.x, p1.x-p3.x, rayDir.x)); // x_a - x_b, x_a - x_c, x_d
	vec3 row2(vec3(p1.y-p2.y, p1.y-p3.y, rayDir.y)); // y_a - y_b, y_a - y_c, y_d
	vec3 row3(vec3(p1.z-p2.z, p1.z-p3.z, rayDir.z)); // z_a - z_b, z_a - z_c, z_d
	float detA = calcualteDeterminant(row1, row2, row3);
	if (detA == 0) // no intersection
		return -1;
	float t = computeT(rayPoint, rayDir, detA);
	// Stop if t is negative or there is already a closer intersection
	if (t < 0 || (ray.getIntersection().hasIntersection() && (t > ray.getIntersection().getDistance())))
		return -1;
	float gamma = computeGamma(rayPoint, rayDir, detA);
	if ((gamma < 0) || gamma > 1)
		return -1;
	float beta = computeBeta(rayPoint, rayDir, detA);
	if ((beta < 0) || beta > 1)
		return -1;
	float alpha = 1 - gamma - beta;
	if ((alpha < 0) || alpha > 1)
		return -1;

	// t is the closest intersection so save it 
	ray.setIntersection(Intersection(1, t, ray.getPoint() + t*rayDir, this));
	return 0;
}

// compute t (intersection distance if it exists) 
float Triangle::computeT(vec3 &p0, vec3 &d, float detA)
{
	vec3 row1(p1.x-p2.x, p1.x-p3.x, p1.x-p0.x);
	vec3 row2(p1.y-p2.y, p1.y-p3.y, p1.y-p0.y);
	vec3 row3(p1.z-p2.z, p1.z-p3.z, p1.z-p0.z);
	return calcualteDeterminant(row1, row2, row3)/detA;
}

// compute the gamma (barycentric coordinate) for the intersection point (if it exists)
float Triangle::computeGamma(vec3 &p0, vec3 &d, float detA)
{
	vec3 row1(p1.x-p2.x, p1.x-p0.x, d.x);
	vec3 row2(p1.y-p2.y, p1.y-p0.y, d.y);
	vec3 row3(p1.z-p2.z, p1.z-p0.z, d.z);
	return calcualteDeterminant(row1, row2, row3)/detA;
}

// compute the beta (barycentric coordinate) for the intersection point (if it exists)
float Triangle::computeBeta(vec3 &p0, vec3 &d, float detA)
{
	vec3 row1(p1.x-p0.x, p1.x-p3.x, d.x);
	vec3 row2(p1.y-p0.y, p1.y-p3.y, d.y);
	vec3 row3(p1.z-p0.z, p1.z-p3.z, d.z);
	return calcualteDeterminant(row1, row2, row3)/detA;
}

// calculates the determinant of a 3x3 matrix
float calcualteDeterminant(vec3 &row1, vec3 &row2, vec3 &row3)
{
	float first = row1.x*row2.y*row3.z - row1.x*row2.z*row3.y;  // aei - afh
	float second = -row1.y*row2.x*row3.z + row1.y*row2.z*row3.x;  // -bdi + bfg
	float third = row1.z*row2.x*row3.y - row1.z*row2.y*row3.x;  // cdh - ceg
	return first + second + third;
}

AABB *Triangle::calculateBBox()
{
	AABB *box = new AABB();
	box->reset(p1);
	box->addPoint(p2);
	box->addPoint(p3);
	return box;
}

/* Taken from Brent Williams */
const vec3 Triangle::getMins() const 
{
   vec3 mins(p1);
   
   if (p2.x < mins.x)
      mins.x = p2.x;
   if (p2.y < mins.y)
      mins.y = p2.y;
   if (p2.z < mins.z)
      mins.z = p2.z;
   
   if (p3.x < mins.x)
      mins.x = p3.x;
   if (p3.y < mins.y)
      mins.y = p3.y;
   if (p3.z < mins.z)
      mins.z = p3.z;
   
   return mins;
}

/* Taken from Brent Williams */
const vec3 Triangle::getMaxs() const
{
   vec3 maxs(p1);
   
   if (p2.x > maxs.x)
      maxs.x = p2.x;
   if (p2.y > maxs.y)
      maxs.y = p2.y;
   if (p2.z > maxs.z)
      maxs.z = p2.z;
   
   if (p3.x > maxs.x)
      maxs.x = p3.x;
   if (p3.y > maxs.y)
      maxs.y = p3.y;
   if (p3.z > maxs.z)
      maxs.z = p3.z;
   
   return maxs;
}

/** Taken from Brent Williams
 * Tests whether the triangle intersects the given axis aligned bounding box (represented by a 
 * minimum corner p and a maximum corner p + deltaP).
 */
bool Triangle::triangleAABBIntersect(vec3& p, vec3& deltaP) const
{
   // Two requirements for intersection
   // 1) The triangle's plane overlaps the AABB
   // 2) For each of the coordinate planes (xy, xz, yz), the 2D projections of the triangle and 
   //    AABB into this plane overlap
   int i;
   //Vec3 c(0,0,0);
   float cx = 0.0f;
   float cy = 0.0f;
   float cz = 0.0f; 
   
   vec3 n(normalize(cross(p2-p1, p3-p1))); 
   float d1;
   float d2;
   vec3 v[3];
   v[0] = p1;
   v[1] = p2;
   v[2] = p3;
   
   float nInnerProductP;
   //nInnerProductP = n.innerProduct(p);
   nInnerProductP = n.x*p.x + n.y*p.y + n.z*p.z;
   
   // 1) The triangle's plane overlaps the AABB
   
   if(n.x > 0)
      cx = deltaP.x;
   if(n.y > 0)
      cy = deltaP.y;
   if(n.z > 0)
      cz = deltaP.z;
   
   //c-v[0]
   float cx_v0x = cx - v[0].x;
   float cy_v0y = cy - v[0].y;
   float cz_v0z = cz - v[0].z;
   
   //d1 = n.innerProduct(c - v[0]);
   d1 = n.x*cx_v0x + n.y*cy_v0y + n.z*cz_v0z;
   
   //(deltaP - c) - v[0]
   float dP_c_v0x = deltaP.x - cx - v[0].x;
   float dP_c_v0y = deltaP.y - cy - v[0].y;
   float dP_c_v0z = deltaP.z - cz - v[0].z;
   
   //d2 = n.innerProduct((deltaP - c) - v[0]);
   d2 = n.x*dP_c_v0x + n.y*dP_c_v0y + n.z*dP_c_v0z;
   
   if ((nInnerProductP + d1) * (nInnerProductP + d2) > 0)
      return false;
   
   
   // 2) For each of the coordinate planes (xy, xz, yz), the 2D projections of the triangle and 
   //    AABB into this plane overlap
   
   //Check for XY
   vec2 pXY;
   vec2 vXY[3];
   vec2 nXY[3];
   vec3 e[3];
   float dXY[3];
   
   pXY.x = p.x;
   pXY.y = p.y;
   
   //printf("pXY: ");
   //pXY.print();
   
   for (i = 0; i <= 2; i++)
   {
      //printf("\n\ni: ", i);
      e[i] = v[(i+1) % 3] - v[i];
      
      //printf("e[%d]: ", i);
      //e[i].print();
      
      nXY[i].x = -1.0 * e[i].y;
      nXY[i].y = e[i].x;
      
      //printf("nXY[%d]: ", i);
      //nXY[i].print();
      
      //printf("n.z: %f\n", n.z);
      if (n.z < 0)
         nXY[i] *= -1.0;
      
      vXY[i].x = v[i].x;
      vXY[i].y = v[i].y;
      
      //printf("vXY[%d]: ", i);
      //vXY[i].print();
      
      //nXY[i].innerProduct(vXY[i]))
      float nXY_iP_vxY = nXY[i].x*vXY[i].x + nXY[i].y*vXY[i].y;
      
      dXY[i] = (-1.0 * nXY_iP_vxY) + std::max(0.0f, deltaP.x*nXY[i].x) + std::max(0.0f, deltaP.y*nXY[i].y); 
      //printf("dXY[%d]: %f\n", i, dXY[i]);
      
      //nXY[i].innerProduct(pXY)
      float nXY_iP_pXY = nXY[i].x*pXY.x + nXY[i].y*pXY.y;
      
      if (nXY_iP_pXY + dXY[i] < 0)
      {
         //printf("Failed on XY\n");
         return false;
      }
   }
   
   //Check for XZ
   vec2 pXZ;
   vec2 vXZ[3];
   vec2 nXZ[3];
   float dXZ[3];
   
   pXZ.x = p.x;
   pXZ.y = p.z;
   
   //printf("pXZ: ");
   //pXZ.print();
   
   for (i = 0; i <= 2; i++)
   {
      //printf("\n\ni: ");
      e[i] = v[(i+1) % 3] - v[i];
      //printf("e[%d]: ", i);
      //e[i].print();
      
      nXZ[i].x = -1.0 * e[i].z;
      nXZ[i].y = e[i].x;
      //printf("nXZ[%d]: ", i);
      //nXZ[i].print();
      
      //printf("n.y: %f\n", n.y);
      if (n.y >= 0)
         nXZ[i] *= -1.0;
      
      vXZ[i].x = v[i].x;
      vXZ[i].y = v[i].z;
      
      //printf("vXZ[%d]: ", i);
      //vXZ[i].print();
      
      //nXZ[i].innerProduct(vXZ[i])
      float nXZ_iP_vXZ = nXZ[i].x*vXZ[i].x + nXZ[i].y*vXZ[i].y;
      
      dXZ[i] = (-1.0 * nXZ_iP_vXZ) + std::max(0.0f, deltaP.x*nXZ[i].x) + std::max(0.0f, deltaP.z*nXZ[i].y); 
      //printf("dXZ[%d]: %f\n", i, dXZ[i]);
      
      //nXZ[i].innerProduct(pXZ)
      float nxZ_iP_pxZ = nXZ[i].x*pXZ.x + nXZ[i].y*pXZ.y;
      if (nxZ_iP_pxZ + dXZ[i] < 0)
      {
         //printf("Failed on XZ\n");
         return false;
      }
   }
   
   
   //Check YZ
   vec2 pYZ;
   vec2 vYZ[3];
   vec2 nYZ[3];
   float dYZ[3];
   
   pYZ.x = p.y;
   pYZ.y = p.z;
   //printf("pYZ: ");
   //pYZ.print();
   
   for (i = 0; i <= 2; i++)
   {
      //printf("\n\ni: ");
      e[i] = v[(i+1) % 3] - v[i];
      //printf("e[%d]: ", i);
      //e[i].print();
      
      nYZ[i].x = -1.0 * e[i].z;
      nYZ[i].y = e[i].y;
      //printf("nYZ[%d]: ", i);
      //nYZ[i].print();
      
      //printf("n.x: %f\n", n.x);
      if (n.x < 0)
         nYZ[i] *= -1.0;
      
      vYZ[i].x = v[i].y;
      vYZ[i].y = v[i].z;
      //printf("vYZ[%d]: ", i);
      //vYZ[i].print();
      
      dYZ[i] = (-1.0 * dot(nYZ[i], vYZ[i])) + std::max(0.0f, deltaP.y*nYZ[i].x) + std::max(0.0f, deltaP.z*nYZ[i].y); 
      //printf("dYZ[%d]: %f\n", i, dYZ[i]);
      
      if (dot(nYZ[i],pYZ) + dYZ[i] < 0)
      {
         //printf("Failed on YZ\n");
         return false;
      }
   }
   
   
   return true;
}

