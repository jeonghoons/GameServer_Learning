#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include "AccountManager.h"
#include "UserManager.h"

void Func1()
{
	for (int i = 0; i < 100; ++i)
	{
		UserManager::Instance()->ProcessSave();
	}
}

void Func2()
{
	for (int i = 0; i < 100; ++i)
	{
		AccountManager::Instance()->ProcessLogin();
	}
}

class SpinLock
{
public:
	void lock()
	{
		// CAS (Compare-And_Swap) 필요
		bool expected = false;
		bool desired = true;

		// CAS 의사 코드
		/*if (_locked == expected) {
			expected = _locked;
			_locked = desired;
			return true;
		}
		else {
			expected = _locked;
			return false;
		}*/


		while (_locked.compare_exchange_strong(expected, desired) == false) {
			expected = false;
		}


		/*while (_locked) {

		}
		_locked = true;*/
	}

	void unlock()
	{
		
		// _locked = false;
		_locked.store(false);
	}

private:
	atomic<bool> _locked = false; // 최적화 하지 않게
};

int sum = 0;
mutex m;
SpinLock spinLock;

void Sum()
{
	for (int i = 0; i < 10'0000; ++i)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub()
{
	for (int i = 0; i < 10'0000; ++i)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}

int main()
{
	/*std::thread t1(Func1);
	std::thread t2(Func2);

	t1.join();
	t2.join();

	cout << "Jobs Done" << endl;*/

	thread t1(Sum);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;

	
}

