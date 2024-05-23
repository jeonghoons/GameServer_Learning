#include "pch.h"
#include "DeadLockProfiler.h"

/*
	DeadLockProfiler
*/

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock);
	
	// 아이디를 찾거나 발급
	int32 lockId = 0;

	auto findit = _nameToId.find(name);
	if (findit == _nameToId.end())
	{
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else {
		lockId = findit->second;
	}

	// 잡고있는 락이 있었을 경우
	if (_lockStack.empty() == false)
	{
		// 기존에 발견되지 않은 케이스라면 데드락 여부확인
		const int32 previd = _lockStack.top();
		if (lockId != previd) {
			set<int32>& history = _lockHistory[previd];
			if (history.find(lockId) == history.end()) {
				history.insert(lockId);
				CheckCycle();
			}
		}
	}

	_lockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (_lockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	int32 lockId = _nameToId[name];
	if (_lockStack.top() != lockId)
		CRASH("INVALID_UNLOCK");

	_lockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	_discoveredOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	_finished = vector<bool>(lockCount, false);
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockid = 0; lockid < lockCount; lockid++)
		Dfs(lockid);

	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockProfiler::Dfs(int32 here)
{
	if (_discoveredOrder[here] != -1)
		return;

	_discoveredOrder[here] = _discoveredCount++;


	// 모든 인접한 정점을 순회
	auto findit = _lockHistory.find(here);
	if (findit == _lockHistory.end()) {
		_finished[here] = true;
		return;
	}

	set<int32>& nextSet = findit->second;
	for (int32 there : nextSet)
	{
		// 아직 방문한 적이 없다면 방문
		if (_discoveredOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		// here가 there보다 먼저 발견되었다면, there은 here의 자식. (순방향 간선)
		if (_discoveredOrder[here] < _discoveredOrder[there])
			continue;

		// 순반향이 아니고, Dfs가 아직 종료되지 않았다면, there는 here의 부모. (역방향간선)
		if (_finished[there] == false) {
			printf("%s -> %s\n", _idToName[here], _idToName[there]);

			int32 now = here;
			while (true) {
				printf("%s -> %s\n", _idToName[_parent[now]], _idToName[now]);
				now = _parent[now];
				if (now == there)
					break;
			}

			CRASH("DEADLOCK_DETECTED");
		}


	}
	_finished[here] = true;
}
