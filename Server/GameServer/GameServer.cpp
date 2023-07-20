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


		while (_locked.compare_exchange_strong(expected, desired) == false) 
		{
			expected = false;

			// 문맥교환 최소화 하기위해 사용
			// 이 쓰레드는 100ms동안 재스케줄링이 되지않는다.
			// this_thread::sleep_for(std::chrono::milliseconds(100)); 
			this_thread::sleep_for(100ms); // 위랑 똑같음 
			// this_thread::yield(); // sleep_for(0ms)와 같음, 언제든지 다시 스케줄링이 될수있지만 반환
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

