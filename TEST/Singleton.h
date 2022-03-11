#pragma once

class Singleton {

private:
	Singleton();
	//static int b;
public:

	static Singleton* instance();

	~Singleton();

	int aa = 100;
}; 
//int Singleton::b = 10;
