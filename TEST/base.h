#pragma once
#include <iostream>
//构造函数和析构函数调虚函数会丢失多态性
class base
{
public:
	base();
	~base();

	void ttest() {
		test();
	}

protected:


	virtual void test() 
	{
		std::cout << "base\n";
	}

private:

};

base::base()
{
	this->test(); 
}

base::~base()
{
}