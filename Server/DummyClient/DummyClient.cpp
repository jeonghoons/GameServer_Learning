﻿#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "BufferReader.h"
#include "ServerPacketHandler.h"

char sendData[] = "Hello World";

class ServerSession : public PacketSession
{
public:
	~ServerSession()
	{
		cout << "~ServerSession" << endl;
	}

	virtual void OnConnected() override
	{
		Protocol::C_LOGIN pkt;
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		Send(sendBuffer);


		// cout << "Connected To Server" << endl;

		/*SendBufferRef sendBuffer = GSendBufferManager->Open(4096);
		::memcpy(sendBuffer->Buffer(), sendData, sizeof(sendData));
		sendBuffer->Close(sizeof(sendData));

		Send(sendBuffer);*/
	}

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		PacketSessionRef session = GetPacketSessionRef();
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

		ServerPacketHandler::HandlerPacket(session, buffer, len);
		
	}

	virtual void OnSend(int32 len) override
	{
		// cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisConnected() override
	{
		// cout << "DisConnected" << endl;
	}
};

int main()
{
	ServerPacketHandler::Init();
	this_thread::sleep_for(1s);

	ClientServiceRef service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,
		2);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 2; ++i)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	Protocol::C_CHAT chatPkt;
	u8string str{ u8"Hello World ! " };
	chatPkt.set_msg(std::string(str.begin(), str.end()));
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(chatPkt);

	while (true)
	{
		service->Broadcast(sendBuffer);
		this_thread::sleep_for(1s);
	}

	GThreadManager->Join();
}