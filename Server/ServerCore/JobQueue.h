#pragma once
#include "Job.h"
#include "LockQueue.h"
#include "JobTimer.h"

class JobQueue : public enable_shared_from_this<JobQueue>
{
public:
	void DoAsync(CallbackType&& callback)
	{
		Push(MakeShared<Job>(std::move(callback)));
	}

	template<typename T, typename Ret, typename... Args>
	void DoAsync(Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		Push(MakeShared<Job>(owner, memFunc, std::forward<Args>(args)...));
	}

	void DoTimer(uint64 tickTimer, CallbackType&& callback)
	{
		// JobRef job = ObjectPool<Job>::MakeShared(std::move(callback));
		JobRef job = MakeShared<Job>(std::move(callback));
		GJobTimer->Reserve(tickTimer, shared_from_this(), job);
	}

	template<typename T, typename Ret, typename... Args>
	void DoTimer(uint64 tickTimer, Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		JobRef job = MakeShared(owner, memFunc, std::forward<Args>(args)...);
		GJobTimer->Reserve(tickTimer, shared_from_this(), job);
	}

	void					ClearJobs() { _jobs.Clear(); }

public:
	void					Push(JobRef job, bool pushOnly = false);
	void					Execute();

protected:
	LockQueue<JobRef>		_jobs;
	Atomic<int32>			_jobCount = 0;
};

