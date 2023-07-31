#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include "ConcurrentQueue.h"
#include "ConcurrentStack.h"

// queue<int32> q;
// stack<int32> s;

// LockQueue<int32> q;
// LockStack<int32> s;

LockQueue<int32> q;
LockFreeStack<int32> s;

void Push()
{
	while (true)
	{
		int32 value = rand() % 100;
		// q.push(value);
		s.Push(value);

		this_thread::sleep_for(10ms);
	}

}

void Pop()
{
	while (true)
	{
		/*if (q.empty())
			continue;
		int32 data = q.front();
		q.pop();
		cout << data << endl; */

		int32 data = 0;
		if (s.TryPop(data))
			cout << data << endl;

	}
}


int main()
{
	thread t1(Push);
	thread t2(Pop);
	thread t3(Pop);


	t1.join();
	t2.join();
	t3.join();
}