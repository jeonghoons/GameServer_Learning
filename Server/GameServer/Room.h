#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
public:
	Room();
	virtual ~Room();

	bool HandleEnterPlayer(PlayerRef player);
	bool HandleLeavePlayer(PlayerRef player);

	void HandleMovePlayer(Protocol::C_MOVE pkt);

	void HandleChat(Protocol::C_CHAT pkt);

public:
	void UpdateTick();

	RoomRef GetRoomRef();

private:
	bool EnterPlayer(PlayerRef player);
	bool LeavePlayer(uint64 objectId);

	
	

	void Broadcast(SendBufferRef sendBuffer, uint64 exceptId = 0);
	
private:
	USE_LOCK;
	unordered_map<uint64, PlayerRef> _players;
};

extern shared_ptr<Room> GRoom;

