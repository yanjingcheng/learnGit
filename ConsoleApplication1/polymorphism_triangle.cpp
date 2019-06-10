#include "pch.h"
#include "polymorphism_triangle.h"


polymorphism_triangle::polymorphism_triangle()
{
}


polymorphism_triangle::~polymorphism_triangle()
{
}

void polymorphism_triangle::area()
{
	std::cout << "triangle class area :" << (length*high / 2) << std::endl;
	return ;
}