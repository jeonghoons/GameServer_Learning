#pragma once

#include <mutex>


template<typename T>
class LockStack
{
public:
	LockStack() {}

	LockStack(const LockStack&) = delete; // 복사생성자 삭제
	LockStack& operator=(const LockStack&) = delete; // 복사생성자 삭제

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value)); // 이왕이면 move를 통해 빠른 연산

		_condVar.notify_one(); // 대기 하고 있는 하나를 깨운다.
	}

	bool TryPop(T& value) // 무한 루프를 통해 검사
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;

		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	// 멀티 쓰레드에서는 empty보장x
	/*bool Empty()
	{
		lock_guard<mutex> lock(_mutex);
		return _stack.empty();
	}*/

	void WaitPop(T& value) // 루프가 아닌 데이터가 들어오면 호출
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _stack.empty() == false; });
		value = std::move(_stack.top());
		_stack.pop();
	}

private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar;
};


template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value), next(nullptr)
		{


		}

		T data;
		Node* next;
	};

public:

	void Push(const T& value)
	{
		Node* node = new Node(value); // 데이터는 힙에 있지만 Node를 참조하는 포인터는 스택영역에 있기에 안전
		node->next = _head;


		/*node->next = _head;
		_head = node;*/  
		// 아토믹하게
		while (_head.compare_exchange_weak(node->next, node) == false)
		{
			// node->next = _head; 알아서 해줌
		}
	}


	// 1) head 읽기
	// 2) head->next 읽기
	// 3) head = head->next
	// 4) data 추출해서 반환
	// 5) 추출한 노드 삭제
	bool TryPop(T& value)
	{
		++_popCount;
		Node* oldHead = _head;

		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false) {

		}

		if (oldHead == nullptr)
		{
			--_popCount;
			return false;
		}

		value = oldHead->data;

		//delete oldHead; // 위험하기 때문에 trydelete
		TryDelete(oldHead);

		return true;
	}

	void TryDelete(Node* oldHead)
	{
		// 자신 외에 누가 있는지
		if (_popCount == 1)
		{
			Node* node = _pendingList.exchange(nullptr);

			if (--_popCount == 0)
			{
				// 중간에 끼어든 애가 없음-> 리스트 삭제 진행
				DeleteNodes(node);
			}
			else if(node)
			{
				// 중간에 끼어든 애가 있으니 삭제 불가 -> 원상 복구
				ChainPendingNodeList(node);
			}

			delete oldHead; // 내 데이터는 삭제
		}
		else
		{
			// 삭제 예약
			ChainPendingNode(oldHead);
			--_popCount;
		}
	}

	static void DeleteNodes(Node* node)
	{
		while (node)
		{
			Node* next = node->next;
			delete node;
			node = next;
		}
	}

	void ChainPendingNodeList(Node* first, Node* last)
	{
		last->next = _pendingList;

		while (_pendingList.compare_exchange_weak(last->next, first) == false)
		{

		}
	}

	void ChainPendingNodeList(Node* node)
	{
		Node* last = node;
		while (last->next)
			last = last->next;

		ChainPendingNodeList(node, last);
	}

	void ChainPendingNode(Node* node)
	{
		ChainPendingNodeList(node, node);
	}

private:
	atomic<Node*> _head;

	atomic<uint32> _popCount = 0; // Pop을 실행중인 쓰레드 개수
	atomic<Node*> _pendingList; // 삭제 되어야 할 노드들의 첫번째
};
