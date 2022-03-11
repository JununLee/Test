#include "Singleton.h"

static Singleton* m_ins = nullptr;
Singleton::Singleton()
{
	aa = 123;
}
Singleton * Singleton::instance()
{


	if (m_ins == nullptr)
	{
		m_ins = new Singleton();
	}

	return m_ins;

}
Singleton::~Singleton()
{
}
