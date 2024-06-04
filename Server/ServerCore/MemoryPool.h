#pragma once

enum
{
	SLIST_ALIGMENT = 16
};

/*
	MemoryHeader
*/

DECLSPEC_ALIGN(SLIST_ALIGMENT)
struct MemoryHeader : public SLIST_ENTRY
{
	// [Header][Data]
	MemoryHeader(int32 size) : allocSize(size) {}

	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size); // placement new
		return reinterpret_cast<void*>(++header);
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}


	int32 allocSize;
};

/*
	MemoryPool
*/

DECLSPEC_ALIGN(SLIST_ALIGMENT)
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void Push(MemoryHeader* ptr);
	MemoryHeader* Pop();

private:
	SLIST_HEADER _header;
	int32 _allocSize = 0;
	atomic<int32> _allocCount = 0;

	/*USE_LOCK;
	queue<MemoryHeader*> _queue;*/
};

