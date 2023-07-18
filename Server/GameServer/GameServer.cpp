#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

vector<int32> v;

// Mutual Exclusive(상호배타적)
mutex m;

// RAII 
template<typename T>
class LockGuard
{
private:
	T* _mutex;

public:
	LockGuard(T& m)
	{
		_mutex = &m;
		_mutex->lock();
	}
	~LockGuard()
	{
		_mutex->unlock();
	}

};

void Push()
{
	for (int32 i = 0; i < 1'0000; ++i)
	{
		// m.lock();

		// LockGuard<std::mutex> lockGuard(m); // 자동으로 소멸하기 위해 락가드
		std::lock_guard<std::mutex> lockGuard(m); // C++에서 지원해주는거

		// std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock); // 당장 락을 잠구진 않고 원하는 시점에서 lock가능
		// uniqueLock.lock();

		v.push_back(i);

		if (i == 5000) {
			// m.unlock();
			break;
		}

		// m.unlock();
	}
}

int main()
{
	v.reserve(2'0000);
	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;
}

