#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

int result;

int64 Calculate()
{
	int64 sum = 0;

	for (int32 i = 0; i < 1000'0000; ++i)
		sum += i;


	return sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret Message");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
}

int main()
{
	// synchronous(동기) 실행
	/*int sum = Calculate();
	cout << sum << endl;*/


	// future 원하는 함수를 비동기적으로 실행
	{
		// 1. deferred -> lazy evaluation 지연해서 나중에 실행, 멀티쓰레드X
		// 2. async -> 별도의 쓰레드를 만들어서 실행
		// 3. deferred | async -> 둘중 아무거나
		std::future<int64> future = std::async(std::launch::async, Calculate);

		// std::future_status status = future.wait_for(1ms);



		int64 sum = future.get(); // 결과물이 이제서야 필요하다.



		//class Knight
		//{
		//public:
		//	int GetHp() { return 100; }
		//};
		//Knight knight;
		//std::future<int> future2 = std::async(std::launch::async, &Knight::GetHp, knight); // knight.GetHP(); 랑 똑같음 :: 쓸일은 거의 없지만 멤버함수도 사용 가능

	}

	// std::promise
	{
		// 미래(future)의 결과물을 반환해줄것이라고 약속(promise)
		std::promise<string> promise;
		std::future<string> future = promise.get_future(); // 쓰레드 통신

		thread t(PromiseWorker, std::move(promise)); // 소유권을 promise에게 넘겨줌

		string message = future.get(); // get()은 딱 한번만 호출 해야됨. 
		cout << message << endl;

		t.join();

	}

	// std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate); // promise와 다른점 -> promise는 타입을 맞춰야되고, task는 호출가능한 것을 넣음
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		cout << sum << endl;

		t.join();

	}

	// 결론
	// mutex, condition_variable까지 가지 않고 단순한 애들을 처리할 수 있는
	// 1회성으로 한 번만 일어나는 이벤트들에 유용

}