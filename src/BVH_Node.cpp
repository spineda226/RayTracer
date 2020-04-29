#include "BVH_Node.h"
#include <algorithm> // std::sort
#include <iostream>
void BVH_Node::recursive_tree_build(std::vector<Shape *> *objects, int axis)
{
	if (objects->size() <= 1)
	{
		this->obj = objects->at(0);
		this->calculateBBox();
		return;
	}
	sort_objects_on_axis(objects, axis);
	
}

void BVH_Node::calculateBBox()
{
	if (this->obj != NULL) // get bbox of the leaf object
	{
		this->box = this->obj->calculateBBox();
	}
	else // combine bbox of left and right
	{
		if (this->left == NULL && this->right == NULL)
			this->box = NULL;
		else if (this->left == NULL && this->right != NULL)
			this->box = this->right->box;
		else if (this->left != NULL && this->right == NULL)
			this->box = this->left->box;
		else
		{
			// Initialize box to left box
			this->box->reset(this->left->box->getMin());
			this->box->addPoint(this->left->box->getMax());
			// Add the right box
			this->box->addBox(*(this->right->box));
		}
	}
}

bool BVH_Node::comparatorX(Shape *i, Shape *j)
{
	return i->getCenter().x < j->getCenter().x;
}

bool BVH_Node::comparatorY(Shape *i, Shape *j)
{
	return i->getCenter().y < j->getCenter().y;
}

bool BVH_Node::comparatorZ(Shape *i, Shape *j)
{
	return i->getCenter().z < j->getCenter().z;
}

// sort objects based on their centers along the given axis
void BVH_Node::sort_objects_on_axis(std::vector<Shape *> *objects, int axis)
{
	int triangles = 0;
	/*
	for (Shape *s : *objects)
	{
		triangles++;
		std::cout << "Shape Center: " << s->getCenter().x << " " << s->getCenter().y << " " << s->getCenter().z << std::endl;
	}
	std::cout << "Triangle count: " << triangles << std::endl;
	*/
	if (axis == XAXIS)
		std::sort(objects->begin(), objects->end(), comparatorX);
	else if (axis == YAXIS)
		std::sort(objects->begin(), objects->end(), comparatorY);
	else if (axis == ZAXIS)
		std::sort(objects->begin(), objects->end(), comparatorZ);
	/*
	std::cout << "Sorting Vector by " << axis << std::endl;
	for (Shape *s : *objects)
	{
		std::cout << "Shape Center: " << s->getCenter().x << " " << s->getCenter().y << " " << s->getCenter().z << std::endl;
	}
	*/
 
}