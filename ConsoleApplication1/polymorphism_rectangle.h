#pragma once
#include "polymorphism_sharp.h"
class polymorphism_rectangle : public polymorphism_sharp
{
public:
	polymorphism_rectangle();
	~polymorphism_rectangle();
	polymorphism_rectangle(int a, int b):polymorphism_sharp(a ,b){}

	void area();
};

