#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>

// atomic : All or Nothing
atomic<int> sum = 0;

void Add()
{
	for (int i = 0; i < 100'0000; ++i)
	{
		sum.fetch_add(1);
		// sum++; // 이것도 문제 없음.
	}
}

void Sub()
{
	for (int i = 0; i < 100'0000; ++i)
	{
		sum.fetch_add(-1);
		// sum--;
	}
}


int main()
{
	Add();
	Sub();
	cout << sum << endl;

	std::thread t1(Add);
	std::thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}

