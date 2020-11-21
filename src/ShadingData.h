#ifndef _SHADINGDATA_H_
#define _SHADINGDATA_H_

#include "PhongMaterial.h"

//#include "Ray.h"

class ShadingData
{
	public:
		ShadingData(const glm::vec3 normal, unsigned int matIdx) : normal(normal), matIdx(matIdx) {}
		~ShadingData() {};

		glm::vec3 getNormal() const { return normal; }
		unsigned int getMatIdx() const { return matIdx; }

	private:
		glm::vec3 normal;
		unsigned int matIdx;
};


#endif