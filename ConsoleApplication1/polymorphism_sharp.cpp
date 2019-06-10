#include "pch.h"
#include "polymorphism_sharp.h"


polymorphism_sharp::polymorphism_sharp()
{
}


polymorphism_sharp::~polymorphism_sharp()
{
}

polymorphism_sharp::polymorphism_sharp(int a, int b)
{
	this->length = a;
	this->high = b;
}

void polymorphism_sharp::area()
{
	std::cout << "parent area:" << std::endl;
	return ;
}
