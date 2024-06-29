#include <iostream>

#include "WeakPtr.hpp"

struct A
{
	A()
	{
		std::cout << "constructing A()\n";
	}

	~A()
	{
		std::cout << "destructing A()\n";
	}
};

int main()
{
	SharedPtr<A> sp(new A);
	SharedPtr<A> sp2 = sp;
	WeakPtr<A> wp(sp2);

	return 0;
}