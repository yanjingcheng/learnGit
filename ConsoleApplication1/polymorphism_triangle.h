#pragma once
#include "polymorphism_sharp.h"

class polymorphism_triangle : public polymorphism_sharp
{
public:
	polymorphism_triangle();
	~polymorphism_triangle();
	polymorphism_triangle(int a, int b) :polymorphism_sharp(a, b) {}

	void area();
};

