#pragma once
#include <iostream>
//���캯���������������麯���ᶪʧ��̬��
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