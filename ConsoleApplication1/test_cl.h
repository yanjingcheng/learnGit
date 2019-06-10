#pragma once
#include <iostream>

class test_cl
{
public:
	test_cl();
	~test_cl();

	const std::string get_male() const { return (int)male ? "famale" : "male"; }
public:
	enum class maleEnum
	{
		male = 0,
		famale = 1
	};

public:
	/*内联函数的定义应该放在 头文件里，内联函数对编译器必须是可见的,不可以放在cpp中，类似C中的宏定义展开*/
	inline void data_print(){ std::cout << "person info:" << name.c_str() << "," << age << "," << get_male().c_str() << std::endl; }

private:
	std::string name;
	maleEnum male;
	int age;
};

