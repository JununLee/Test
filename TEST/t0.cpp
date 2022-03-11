#include "t0.h"
#include <iostream>

void t0::t_cb4()
{
	std::cout << "cb4\n";
}

t0::t0()
{
}

t0::~t0()
{
}

void t0::t0_F0(int test[1])
{
	test[0] = 123;
}

void t0::t0_F1()
{
	std::cout << test[0] << "\n";
}

void t0::t0_F2()
{
	cb0();
}

void t0::t0_F3(CallBack1 cb)
{
	cb(123);
}

void t0::t0_F4(CallBack2 cb)
{
	int sum = cb(123, 456,789);
	std::cout << sum << "\n";
}

void t0::t0_F5()
{
	int res = cb3();
	std::cout << res << "\n";
}

void t0::t0_F6()
{
	cb4();
}

void t0::t0_F7()
{
	cb2(2,3,4);
}
