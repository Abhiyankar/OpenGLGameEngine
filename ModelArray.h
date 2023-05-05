#ifndef MODELARRAY_H
#define MODELARRAY_H

#include "BasicCube.h"

template <class T>
class ModelArray
{
	std::vector <RigidBody> instances;

	void init()
	{
		model.init();
	}

protected:
	T model;
};

#endif
