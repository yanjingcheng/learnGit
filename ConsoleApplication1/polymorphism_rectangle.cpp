#include "pch.h"
#include "polymorphism_rectangle.h"


polymorphism_rectangle::polymorphism_rectangle()
{
}


polymorphism_rectangle::~polymorphism_rectangle()
{
}

void polymorphism_rectangle::area()
{	
	std::cout << "rectangle class area:" << length * high << std::endl;
	return ;
}