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

void Producer()
{
	while (true)
	{
		
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		::SetEvent(handle); // 이벤트의 시그널상태 변경

		this_thread::sleep_for(3s);
	}

	
}

void Consumer()
{
	while (true)
	{
		// 무한루프를 도는게 아니라 (set을)대기하여 무의미하게 실행되는것을 막아줌
		::WaitForSingleObject(handle, INFINITE); // 이벤트가 발생하여 시그널이 바뀌면 깨운다.
		// 그리고 오토리셋이기 때문에 다시 non-signal로 변경됨-> 오토가 아니면 ResetEvent(handle)로 바꿔줘야됨


		unique_lock<mutex> lock(m);
		if (q.empty() == false) {
			int data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}

int main()
{
	// 커널 오브젝트
	handle = ::CreateEvent(NULL, FALSE/*자동리셋*/, FALSE/*초기값(시그널)*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}

