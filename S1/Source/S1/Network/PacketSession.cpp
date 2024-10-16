// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/PacketSession.h"
#include "NetworkWorker.h"
#include "Sockets.h"
#include "ServerPacketHandler.h"

PacketSession::PacketSession(class FSocket* Socket) : Socket(Socket)
{
	ServerPacketHandler::Init();
}

PacketSession::~PacketSession()
{
	Disconnect();
}

void PacketSession::Run()
{
	RecvWorkerThread = MakeShared<RecvWorker>(Socket, AsShared());
	SendWorkerThread = MakeShared<SendWorker>(Socket, AsShared());
}

void PacketSession::HandleRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (RecvPacketQueue.Dequeue(Packet) == false)
			break;

		PacketSessionRef ThisPtr = AsShared();

		ServerPacketHandler::HandlerPacket(ThisPtr, Packet.GetData(), Packet.Num());
	}
}

void PacketSession::SendPacket(SendBufferRef Sendbuffer)
{
	SendPacketQueue.Enqueue(Sendbuffer);
}

void PacketSession::Disconnect()
{
	if (RecvWorkerThread)
	{
		RecvWorkerThread->Destroy();
		RecvWorkerThread = nullptr;
	}
	if (SendWorkerThread)
	{
		SendWorkerThread->Destroy();
		SendWorkerThread = nullptr;
	}
}
