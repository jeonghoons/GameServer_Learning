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

int32 GameSession::OnRecv(BYTE* buffer, int32 len)
{
	cout << "OnRecv Len = " << len << endl;

	SendBufferRef sendBuffer = GSendBufferManager->Open(4096);
	::memcpy(sendBuffer->Buffer(), buffer, len);
	sendBuffer->Close(len);

	Send(sendBuffer);

	GSessionManager.Broadcast(sendBuffer);

	return len;
}

void GameSession::OnSend(int32 len)
{
	cout << "OnSend Len = " << len << endl;
}