#ifndef _PIGMENT_H_
#define _PIGMENT_H_

class Pigment
{
   public:
      Pigment() {};
      Pigment(float r, float g, float b);
      float getR() const { return r; }
      float getG() const { return g; }
      float getB() const { return b; }

   private:
      float r;
      float g;
      float b;
};

#endif
