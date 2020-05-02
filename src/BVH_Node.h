#ifndef _BVHNODE_H_
#define _BVHNODE_H_

#include "Shape.h"
#include "AABB.h"
#include <vector>
#include <glm/glm.hpp>
using namespace glm;

#define XAXIS 0
#define YAXIS 1
#define ZAXIS 2

class BVH_Node
{
   public:
      BVH_Node() : left(NULL), right(NULL), obj(NULL) { box = new AABB(); }
      BVH_Node(BVH_Node *left, BVH_Node *right, Shape *obj) : left(left), right(right), obj(obj) {}
      ~BVH_Node() {};

      BVH_Node *getLeft() const { return left; }
      BVH_Node *getRight() const { return right; }
      Shape *getObj() const { return obj; }

      void recursive_tree_build(std::vector<Shape *> *objects, int startIdx, int endIdx, int axis);
      static void sort_objects_on_axis(std::vector<Shape *> *objects, int startIdx, int endIdx, int axis);
      void calculateBBox();

      static bool comparatorX(Shape *i, Shape *j);
      static bool comparatorY(Shape *i, Shape *j);
      static bool comparatorZ(Shape *i, Shape *j);

      int getClosestIntersection(Ray &ray) const;
      bool hit(Ray &ray) const;

   private:
      BVH_Node *left;
      BVH_Node *right;
      Shape *obj;
      AABB *box;
};

#endif