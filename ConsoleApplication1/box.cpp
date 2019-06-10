#include "pch.h"
#include "box.h"


box::box()
{
}


box::~box()
{
}


box box::operator+(const box& _inBox) 
{
	box valid_box;
	std::cout << "testdddd:" << this->length << std::endl;
	valid_box.length = this->length + _inBox.length;
	valid_box.high = this->high + _inBox.high;
	valid_box.width = this->width + _inBox.width;

	return valid_box;
}
