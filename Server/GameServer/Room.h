#pragma once
#include "JobQueue.h"

class Room : public enable_shared_from_this<Room>
{
public:
	Room();
	virtual ~Room();

	bool HandleEnterPlayerLocked(PlayerRef player);
	bool HandleLeavePlayerLocked(PlayerRef player);

	void HandleMovePlayerLocked(Protocol::C_MOVE& pkt);

private:
	bool EnterPlayer(PlayerRef player);
	bool LeavePlayer(uint64 objectId);
	

	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);
	
private:
	USE_LOCK;
	unordered_map<uint64, PlayerRef> _players;
};

extern shared_ptr<Room> GRoom;

