#include "AABB.h"

// initializes the min and max to pt
void AABB::reset(vec3 pt)
{
	min = max = pt;
}

// recalculates the bounds of the AABB given a new point
void AABB::addPoint(vec3 pt)
{
	min.x = glm::min(min.x, pt.x);
	min.y = glm::min(min.y, pt.y);
	min.z = glm::min(min.z, pt.z);
	max.x = glm::max(max.x, pt.x);
	max.y = glm::max(max.y, pt.y);
	max.z = glm::max(max.z, pt.z);
}

// combines two AABB into a bigger one
void AABB::addBox(AABB other)
{
	addPoint(other.min);
	addPoint(other.max);
}

bool AABB::epsilonEquals(float const a, float const b, float const epsilon)
{
    return std::abs(a - b) < epsilon;
}

// returns true or false if the ray hits the box
// does not update the ray's intersection point
bool AABB::hit(const Ray &r)
{
	vec3 rayPoint = r.getPoint();
	vec3 rayDir = r.getDir();
	float txmin, tymin, tzmin, txmax, tymax, tzmax, temp;
	txmin = tymin = tzmin = -1*std::numeric_limits<float>::infinity();
	txmax = tymax = tzmax = std::numeric_limits<float>::infinity();
	// Return false if ray is parallel to an axis and won't intersect
	if (epsilonEquals(rayDir.x, 0))
	{
		if (rayPoint.x < min.x || rayPoint.x > max.x)
			return false;
	}
	else
	{
		txmin = (min.x - rayPoint.x)/rayDir.x;
		txmax = (max.x - rayPoint.x)/rayDir.x;
		if (txmin > txmax)
		{
			temp = txmin;
			txmin = txmax;
			txmax = temp;
		}
	}

	// y
	if (epsilonEquals(rayDir.y, 0))
	{
		if (rayPoint.y < min.y || rayPoint.y > max.y)
			return false;
	}
	else
	{
		tymin = (min.y - rayPoint.y)/rayDir.y;
		tymax = (max.y - rayPoint.y)/rayDir.y;
		if (tymin > tymax)
		{
			temp = tymin;
			tymin = tymax;
			tymax = temp;
		}
	}

	// z
	if (epsilonEquals(rayDir.z, 0))
	{
		if (rayPoint.z < min.z || rayPoint.z > max.z)
			return false;
	}
	else
	{
		tzmin = (min.z - rayPoint.z)/rayDir.z;
		tzmax = (max.z - rayPoint.z)/rayDir.z;
		if (tzmin > tzmax)
		{
			temp = tzmin;
			tzmin = tzmax;
			tzmax = temp;
		}
	}

	float smallestMax, largestMin;
	smallestMax = glm::min(glm::min(txmax, tymax), tzmax);
	largestMin = glm::max(glm::max(txmin, tymin), tzmin);

	// If max of mins is > min of maxes, then miss
	if (smallestMax < largestMin || smallestMax < 0)
		return false;
	return true;
}


// returns true or false if the ray hits the box
// does not update the ray's intersection point
bool AABB::hit(const Ray &r, float &t)
{
	vec3 rayPoint = r.getPoint();
	vec3 rayDir = r.getDir();
	float txmin, tymin, tzmin, txmax, tymax, tzmax, temp;
	txmin = tymin = tzmin = -1*std::numeric_limits<float>::infinity();
	txmax = tymax = tzmax = std::numeric_limits<float>::infinity();
	// Return false if ray is parallel to an axis and won't intersect
	if (epsilonEquals(rayDir.x, 0))
	{
		if (rayPoint.x < min.x || rayPoint.x > max.x)
			return false;
	}
	else
	{
		txmin = (min.x - rayPoint.x)/rayDir.x;
		txmax = (max.x - rayPoint.x)/rayDir.x;
		if (txmin > txmax)
		{
			temp = txmin;
			txmin = txmax;
			txmax = temp;
		}
	}

	// y
	if (epsilonEquals(rayDir.y, 0))
	{
		if (rayPoint.y < min.y || rayPoint.y > max.y)
			return false;
	}
	else
	{
		tymin = (min.y - rayPoint.y)/rayDir.y;
		tymax = (max.y - rayPoint.y)/rayDir.y;
		if (tymin > tymax)
		{
			temp = tymin;
			tymin = tymax;
			tymax = temp;
		}
	}

	// z
	if (epsilonEquals(rayDir.z, 0))
	{
		if (rayPoint.z < min.z || rayPoint.z > max.z)
			return false;
	}
	else
	{
		tzmin = (min.z - rayPoint.z)/rayDir.z;
		tzmax = (max.z - rayPoint.z)/rayDir.z;
		if (tzmin > tzmax)
		{
			temp = tzmin;
			tzmin = tzmax;
			tzmax = temp;
		}
	}

	float smallestMax, largestMin;
	smallestMax = glm::min(glm::min(txmax, tymax), tzmax);
	largestMin = glm::max(glm::max(txmin, tymin), tzmin);

	// If max of mins is > min of maxes, then miss
	if (smallestMax < largestMin || smallestMax < 0)
		return false;


	if (largestMin > 0)
		t = largestMin;
	else
		t = smallestMax;

	return true;
}

// Taken from Brent Williams
// Turns bounding box into a square
void AABB::square()
{
   float xLen = max.x - min.x;
   float yLen = max.y - min.y;
   float zLen = max.z - min.z;
   float largest = xLen;
   
   if (yLen > largest)
      largest = yLen;
   if (zLen > largest)
      largest = zLen;

   float halfLength = largest * 0.5f; // should this be largest?

   float centerX = (min.x + max.x) * 0.5f;
   min.x = centerX - halfLength;
   max.x = centerX + halfLength;

   float centerY = (min.y + max.y) * 0.5f;
   min.y = centerY - halfLength;
   max.y = centerY + halfLength;

   float centerZ = (min.z + max.z) * 0.5f;
   min.z = centerZ - halfLength;
   max.z = centerZ + halfLength;
}

bool AABB::contains(const vec3& val)
{
   for (int i = 0; i < 3; i++)
   {
      if (val[i] < min[i] || val[i] > max[i])
         return false;
   }
   return true;
}

bool AABB::inRange(float val, float min_val, float max_val)
{
   return val >= min_val && val <= max_val;
}

bool AABB::intersect(const Ray& ray, float& t, vec3& normal)
{
   float normalDirection[3] = {1.0f,1.0f,1.0f};
   normal = vec3(0.0f,0.0f,0.0f);
   //cout << "AABB: ";
   //print();
   //cout << "Ray: ";
   //ray.print();
   // If the ray starts inside the box, or ends inside
   if (contains(ray.getPoint()))
   {
      return false; 
   }

   // the algorithm says, find 3 t's,
   vec3 tVals;

   // LARGEST t is the only one we need to test if it's on the face.
   for (int i = 0 ; i < 3 ; i++)
   {
      if (ray.getDir()[i] == 0)
      {
         tVals[i] = -FLT_MAX;
      }
      else if(ray.getDir()[i] > 0) // CULL BACK FACE
      {
         tVals[i] = ( min[i] - ray.getPoint()[i] ) / ray.getPoint()[i];
         
         // If the ray direction is positive, it will hit the more negative AABB side which makes 
         // the normal point in the negative direction
         normalDirection[i] = -1.0f; 
      }
      else
      {
         tVals[i] = ( max[i] - ray.getPoint()[i] ) / ray.getDir()[i];

         // If the ray direction is positive, it will hit the more positive AABB side which makes 
         // the normal point in the positive direction
         normalDirection[i] = 1.0f; 
      }
   }

   // cout << "tVals: ";
   // for (int i = 0; i < 3; ++i)
   // {
   //    cout << "(" << i << ": " << tVals[i] << ")  ";
   // }
   // cout << endl;

   // Get the max index
   float maxValue = tVals[0];
   int maxIndex = 0;
   if (tVals[1] > maxValue)
   {
      maxValue = tVals[1];
      maxIndex = 1;
   }
   
   if (tVals[2] > maxValue)
   {
      maxValue = tVals[2];
      maxIndex = 2;
   }

   t = maxValue;
   normal[maxIndex] = normalDirection[maxIndex];

   if (tVals[maxIndex] > 0.0f)
   {
      vec3 pt = ray.at(tVals[maxIndex]);

      // check it's in the box in other 2 dimensions
      int o1 = ( maxIndex + 1 ) % 3; // i=0: o1=1, o2=2, i=1: o1=2,o2=0 etc.
      int o2 = ( maxIndex + 2 ) % 3;

      return inRange(pt[o1], min[o1], max[o1]) && inRange(pt[o2], min[o2], max[o2]) ;
   }

   return false ; // the ray did not hit the box.
}