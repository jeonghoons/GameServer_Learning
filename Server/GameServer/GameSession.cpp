#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"


void GameSession::OnConnected()
{
	GSessionManager.ADD(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnDisConnected()
{
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));
}

int32 GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	// cout << "OnRecv Len = " << len << endl;

	PacketHeader header = *((PacketHeader*)buffer);
	cout << "Packet ID : " << header.id << " , Size : " << header.size << endl;

	//SendBufferRef sendBuffer = GSendBufferManager->Open(4096);
	//::memcpy(sendBuffer->Buffer(), buffer, len);
	//sendBuffer->Close(len);

	//// Send(sendBuffer);

	//GSessionManager.Broadcast(sendBuffer);

	return len;
}

void GameSession::OnSend(int32 len)
{
	// cout << "OnSend Len = " << len << endl;
}