#ifndef _VECTOR_H_
#define _VECTOR_H_

class Vector
{
   public:
      Vector() { x=y=z=0; };
      Vector(float x, float y, float z);
      ~Vector();
      float getX() const { return x; }
      float getY() const { return y; }
      float getZ() const { return z; }

   private:
      float x;
      float y;
      float z;
};

#endif
