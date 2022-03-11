#pragma once
#include <vector>
class vectorTest
{
public:
	vectorTest();
	~vectorTest();

	void test(std::vector<double> point);

	void print();

private:

	std::vector<double> mpoint;
}; 
