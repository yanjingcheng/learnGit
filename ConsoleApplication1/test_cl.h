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
	/*���������Ķ���Ӧ�÷��� ͷ�ļ�����������Ա����������ǿɼ���,�����Է���cpp�У�����C�еĺ궨��չ��*/
	inline void data_print(){ std::cout << "person info:" << name.c_str() << "," << age << "," << get_male().c_str() << std::endl; }

private:
	std::string name;
	maleEnum male;
	int age;
};

