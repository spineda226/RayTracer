#include "BVH_Node.h"

void BVH_Node::recursive_tree_build(std::vector<Shape *> *objects)
{
	if (objects->size() <= 1)
	{
		this->obj = objects->at(0);
		this->calculateBBox();
	}
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