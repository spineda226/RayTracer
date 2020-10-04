#include "BVH_Node.h"
#include <algorithm> // std::sort
#include <iostream>
#include <easy/profiler.h>

// return immediately if there is a hit
bool BVH_Node::hit(Ray &ray) const
{
	if (this->box->hit(ray))
	{
		if (this->obj != NULL)
		{
			this->obj->getClosestIntersection(ray);
			if (ray.getIntersection().hasIntersection() == 1)
				return true;
			return false;
		}
		else
		{
			if (this->left->hit(ray))
				return true;
			if (this->right->hit(ray))
				return true;
		}

	}
	return false;
}
// Gets closests intersection in the BVH 
int BVH_Node::getClosestIntersection(Ray &ray) const
{
	if (this->box->hit(ray)) // hits the root 
	{
		if (this->obj != NULL) // Leaf child (actual geometry)
		{
			this->obj->getClosestIntersection(ray);
		}
		else // check left and right
		{
			this->left->getClosestIntersection(ray);
			this->right->getClosestIntersection(ray);
		}
		if (ray.getIntersection().hasIntersection() == 1)
			return 0;
	}
	return -1; // missed the root
}

void BVH_Node::recursive_tree_build(std::vector<Shape *> *objects, int startIdx, int endIdx, int axis)
{
	EASY_FUNCTION(profiler::colors::Red); // Magenta block with name "foo"
	int N = endIdx - startIdx + 1;
	if (N <= 1)
	{
		//std::cout << "assigning leaf at idx: " << startIdx << std::endl;
		this->obj = objects->at(startIdx);
		this->calculateBBox();
		//std::cout << "bbox min: " << this->box->getMin().x << " " << this->box->getMin().y << " " << this->box->getMin().z << std::endl;
		//std::cout << "bbox max: " << this->box->getMax().x << " " << this->box->getMax().y << " " << this->box->getMax().z << std::endl;
		return;
	}
	sort_objects_on_axis(objects, startIdx, endIdx, axis);
	
	this->left = new BVH_Node();
	this->right = new BVH_Node();

	this->left->recursive_tree_build(objects, startIdx, startIdx + N/2 - 1, (axis+1)%3);
	this->right->recursive_tree_build(objects, startIdx + N/2, endIdx, (axis+1)%3);
	this->calculateBBox();
	/*
	std::cout << "recalculating bbox for " << startIdx << " to " << endIdx <<  std::endl;
	std::cout << "bbox min: " << this->box->getMin().x << " " << this->box->getMin().y << " " << this->box->getMin().z << std::endl;
	std::cout << "bbox max: " << this->box->getMax().x << " " << this->box->getMax().y << " " << this->box->getMax().z << std::endl;
	*/
}

void BVH_Node::calculateBBox()
{
	EASY_FUNCTION();
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
void BVH_Node::sort_objects_on_axis(std::vector<Shape *> *objects, int startIdx, int endIdx, int axis)
{
	int numElements = endIdx-startIdx + 1;
	/*
	int triangles = 0;
	for (Shape *s : *objects)
	{
		triangles++;
		std::cout << "Shape Center: " << s->getCenter().x << " " << s->getCenter().y << " " << s->getCenter().z << std::endl;
	}
	std::cout << "Triangle count: " << triangles << std::endl;
	*/
	if (axis == XAXIS)
		std::sort(objects->begin()+startIdx, objects->begin() + startIdx + numElements, comparatorX);
	else if (axis == YAXIS)
		std::sort(objects->begin()+startIdx, objects->begin() + startIdx + numElements, comparatorY);
	else if (axis == ZAXIS)
		std::sort(objects->begin()+startIdx, objects->begin() + startIdx + numElements, comparatorZ);
	/*
	std::cout << "Sorting Vector by " << axis << std::endl;
	for (Shape *s : *objects)
	{
		std::cout << "Shape Center: " << s->getCenter().x << " " << s->getCenter().y << " " << s->getCenter().z << std::endl;
	}
	*/
}