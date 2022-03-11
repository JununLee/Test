#pragma once
#include <functional>
#include <string>


typedef std::function<void()> CallBack0;
typedef std::function<void(int)> CallBack1;
typedef std::function<int(int, int, int)> CallBack2;
typedef std::function<int()> CallBack3;

class t0
{
public:
	CallBack0 cb0;
	CallBack1 cb1;
	CallBack2 cb2;
	CallBack3 cb3;
	CallBack0 cb4;
	CallBack2 cb5;

	void t_cb4();

	int test[1];

	t0();
	~t0();

	void t0_F0(int test[1]);
	void t0_F1();

	void t0_F2();
	void t0_F3(CallBack1 cb);
	void t0_F4(CallBack2 cb);
	void t0_F5(); 
	void t0_F6();
	void t0_F7();

private:

	

};
