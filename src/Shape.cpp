#include "Shape.h"

Shape::Shape()
{
	color = vec3(0);
	filter = 0;
	finish = new properties{0.1, 0.6, 0, 0.05, 0, 0, 1.0};
}