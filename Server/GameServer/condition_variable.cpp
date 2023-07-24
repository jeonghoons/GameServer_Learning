#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

mutex m;
queue<int> q;
HANDLE handle;

// 참고) CV는 User-Level Object (커널X)
condition_variable cv; // 뮤텍스랑 짝지어서 동작

void Producer()
{
	while (true)
	{
		// 1) Lock을 잡고
		// 2) 공유 변수 값을 수정
		// 3) Lock을 풀고
		// 4) 조건변수를 통해 다른 쓰레드에게 통지


		{
			unique_lock<mutex> lock(m); // [1]
			q.push(100); // [2]
		} // [3] 유니크 락이기 때문에 자동으로 Lock을 풀어줌 

		// ::SetEvent(handle); // 이벤트의 시그널상태 변경
		cv.notify_one(); // [4] wait중인 쓰레드가 있으면 딱 1개를 깨운다.

		// this_thread::sleep_for(3s);
	}

	
}

void Consumer()
{
	while (true)
	{
		// 무한루프를 도는게 아니라 (set을)대기하여 무의미하게 실행되는것을 막아줌
		// ::WaitForSingleObject(handle, INFINITE); // 이벤트가 발생하여 시그널이 바뀌면 깨운다.
		// 그리고 오토리셋이기 때문에 다시 non-signal로 변경됨-> 오토가 아니면 ResetEvent(handle)로 바꿔줘야됨


		unique_lock<mutex> lock(m);

		cv.wait(lock, []() {return q.empty() == false; });
		// 1) Lock을 잡고
		// 2) 조건 확인
		//  - 만족 O -> 빠져 나와서 이어서 코드를 진행
		//  - 만족 X -> Lock을 풀어주고 대기 상태로 바꿔줌

		// Spurious Wakeup: notify_one을 할 때 lock을 잡고 있는것이 아니기 때문에 항상 조건식을 만족하는건 아님

		// while (q.empty() == false) 
		{
			int data = q.front();
			q.pop();
			cout << q.size() << endl;
		}
	}
}

int main()
{
	// 커널 오브젝트
	// handle = ::CreateEvent(NULL, FALSE/*자동리셋*/, FALSE/*초기값(시그널)*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	// ::CloseHandle(handle);
}

