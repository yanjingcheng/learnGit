#pragma once
class box
{
public:
	box();
	~box();
	
public:
	int getArea() { return this->length * this->width * this->high; }
	inline void setLength(int _len) { this->length = _len; }
	inline void setWidth(int _width) { this->width = _width; }
	inline void setHigh(int _high) { this->high = _high; }

public:
	box operator+(const box& _inBox);
	
private:
	int length;
	int width;
	int high;
};

