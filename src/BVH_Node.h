#ifndef _BVHNODE_H_
#define _BVHNODE_H_

#include "Shape.h"
#include "AABB.h"
#include <vector>
#include <glm/glm.hpp>
using namespace glm;

class BVH_Node
{
   public:
      BVH_Node() : left(NULL), right(NULL), obj(NULL) { box = new AABB(); }
      BVH_Node(BVH_Node *left, BVH_Node *right, Shape *obj) : left(left), right(right), obj(obj) {}
      ~BVH_Node() {};

      BVH_Node *getLeft() const { return left; }
      BVH_Node *getRight() const { return right; }
      Shape *getObj() const { return obj; }

      void recursive_tree_build(std::vector<Shape *> *objects);
      void calculateBBox();

   private:
      BVH_Node *left;
      BVH_Node *right;
      Shape *obj;
      AABB *box;
};

#endif