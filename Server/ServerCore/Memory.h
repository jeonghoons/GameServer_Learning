#pragma once
#include "Allocator.h"

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

