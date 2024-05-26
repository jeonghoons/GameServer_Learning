#pragma once
#include "Allocator.h"

// ...�� ���� ����
template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(xxalloc(sizeof(Type)));
	
	// placement new
	new(memory)Type(forward<Args>(args)...); // �޸𸮴� ���� �Ҵ��ϰ�, �����ڸ� ȣ��
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	xxrelease(obj);
}

