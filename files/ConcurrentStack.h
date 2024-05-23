#pragma once

#include <mutex>


template<typename T>
class LockStack
{
public:
	LockStack() {}

	LockStack(const LockStack&) = delete; // ��������� ����
	LockStack& operator=(const LockStack&) = delete; // ��������� ����

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value)); // �̿��̸� move�� ���� ���� ����

		_condVar.notify_one(); // ��� �ϰ� �ִ� �ϳ��� �����.
	}

	bool TryPop(T& value) // ���� ������ ���� �˻�
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;

		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	// ��Ƽ �����忡���� empty����x
	/*bool Empty()
	{
		lock_guard<mutex> lock(_mutex);
		return _stack.empty();
	}*/

	void WaitPop(T& value) // ������ �ƴ� �����Ͱ� ������ ȣ��
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
		Node* node = new Node(value); // �����ʹ� ���� ������ Node�� �����ϴ� �����ʹ� ���ÿ����� �ֱ⿡ ����
		node->next = _head;


		/*node->next = _head;
		_head = node;*/  
		// ������ϰ�
		while (_head.compare_exchange_weak(node->next, node) == false)
		{
			// node->next = _head; �˾Ƽ� ����
		}
	}


	// 1) head �б�
	// 2) head->next �б�
	// 3) head = head->next
	// 4) data �����ؼ� ��ȯ
	// 5) ������ ��� ����
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

		//delete oldHead; // �����ϱ� ������ trydelete
		TryDelete(oldHead);

		return true;
	}

	void TryDelete(Node* oldHead)
	{
		// �ڽ� �ܿ� ���� �ִ���
		if (_popCount == 1)
		{
			Node* node = _pendingList.exchange(nullptr);

			if (--_popCount == 0)
			{
				// �߰��� ����� �ְ� ����-> ����Ʈ ���� ����
				DeleteNodes(node);
			}
			else if(node)
			{
				// �߰��� ����� �ְ� ������ ���� �Ұ� -> ���� ����
				ChainPendingNodeList(node);
			}

			delete oldHead; // �� �����ʹ� ����
		}
		else
		{
			// ���� ����
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

	atomic<uint32> _popCount = 0; // Pop�� �������� ������ ����
	atomic<Node*> _pendingList; // ���� �Ǿ�� �� ������ ù��°
};
