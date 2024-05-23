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
	// synchronous(����) ����
	/*int sum = Calculate();
	cout << sum << endl;*/


	// future ���ϴ� �Լ��� �񵿱������� ����
	{
		// 1. deferred -> lazy evaluation �����ؼ� ���߿� ����, ��Ƽ������X
		// 2. async -> ������ �����带 ���� ����
		// 3. deferred | async -> ���� �ƹ��ų�
		std::future<int64> future = std::async(std::launch::async, Calculate);

		// std::future_status status = future.wait_for(1ms);



		int64 sum = future.get(); // ������� �������� �ʿ��ϴ�.



		//class Knight
		//{
		//public:
		//	int GetHp() { return 100; }
		//};
		//Knight knight;
		//std::future<int> future2 = std::async(std::launch::async, &Knight::GetHp, knight); // knight.GetHP(); �� �Ȱ��� :: ������ ���� ������ ����Լ��� ��� ����

	}

	// std::promise
	{
		// �̷�(future)�� ������� ��ȯ���ٰ��̶�� ���(promise)
		std::promise<string> promise;
		std::future<string> future = promise.get_future(); // ������ ���

		thread t(PromiseWorker, std::move(promise)); // �������� promise���� �Ѱ���

		string message = future.get(); // get()�� �� �ѹ��� ȣ�� �ؾߵ�. 
		cout << message << endl;

		t.join();

	}

	// std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate); // promise�� �ٸ��� -> promise�� Ÿ���� ����ߵǰ�, task�� ȣ�Ⱑ���� ���� ����
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		cout << sum << endl;

		t.join();

	}

	// ���
	// mutex, condition_variable���� ���� �ʰ� �ܼ��� �ֵ��� ó���� �� �ִ�
	// 1ȸ������ �� ���� �Ͼ�� �̺�Ʈ�鿡 ����

}