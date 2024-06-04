#pragma once
#include "Allocator.h"

class MemoryPool;

/*
	MemoryManager
*/
class MemoryManager
{
	enum
	{
		// ~1024 - 32단위, ~2048 - 128단위, ~4096 - 256단위
		POOL_COUNT = (1024 / 32) + (1024 / 128) + (2048 / 256),
		MAX_ALLOC_SIZE = 4096
	};

public:
	MemoryManager();
	~MemoryManager();

	void* Allocate(int32 size);
	void Release(void* ptr);

private:
	vector<MemoryPool*> _pools;

	MemoryPool* _poolTable[MAX_ALLOC_SIZE + 1]; // O(1)

};

// ...은 가변 길이
template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xxalloc(sizeof(Type)));
	
	// placement new
	new(memory)Type(forward<Args>(args)...); // 메모리는 직접 할당하고, 생성자를 호출
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	xxrelease(obj);
}

