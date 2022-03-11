#pragma once
#include "base.h"
class sun : public base
{
public:
	sun();
	~sun();

protected:
	virtual void test() override
	{
		std::cout << "sun\n";
	}
private:

};

sun::sun():base()
{ 
	ttest();
}

sun::~sun()
{
}