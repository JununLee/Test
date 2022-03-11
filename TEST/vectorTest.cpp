#include "vectorTest.h"
#include <iostream>

vectorTest::vectorTest()
{
}

vectorTest::~vectorTest()
{
}

void vectorTest::test(std::vector<double> point)
{
	mpoint = point;
}

void vectorTest::print()
{
	std::cout << mpoint[0] << "\n";
}
