#pragma once

struct JobData
{
	JobData(weak_ptr<JobQueue> owner, JobRef job) : owner(owner), job(job) {}

	weak_ptr<JobQueue> owner; // shared_ptr로 하기엔 생명주기가 애매해서 weak로
	JobRef job;
};

struct TimerItem
{
	bool operator<(const TimerItem& other) const
	{
		return executeTick > other.executeTick;
	}

	uint64 executeTick = 0;
	JobData* jobData = nullptr;
};

/*
	JobTimer
*/

class JobTimer
{
public:
	void Reserve(uint64 tickAfer, weak_ptr<JobQueue> owner, JobRef job);
	void Distribute(uint64 now);
	void Clear();

private:
	USE_LOCK;
	PriorityQueue<TimerItem> _items;
	Atomic<bool> _distributing = false;

};

