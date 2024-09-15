#include "pch.h"
#include "ThreadManager.h"
#include "Session.h"
#include "Service.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ServerPacketHandler.h"

class GameSession2 : public Session
{
public:
	~GameSession2()
	{
		cout << "~GameSession" << endl;
	}
	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		cout << "OnRecv Len = " << len << endl;

		SendBufferRef sendBuffer = GSendBufferManager->Open(4096);
		::memcpy(sendBuffer->Buffer(), buffer, len);
		sendBuffer->Close(len);

		Send(sendBuffer);
		  
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}
};

int main()
{
	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>,
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	char sendData[1000] = "Hello World";
	while (true)
	{
		//SendBufferRef sendBuffer = GSendBufferManager->Open(4096);

		//BufferWriter bw(sendBuffer->Buffer(), 4096);

		//PacketHeader* header = bw.Reserve<PacketHeader>();

		//bw << (uint64)1001 << (uint32)100 << (uint16)10; // ex) id, 체력, 공격력
		//bw.Write(sendData, sizeof(sendData));

		///*header->size = sizeof(sendData) + sizeof(PacketHeader);
		//header->id = 1;*/

		//header->size = bw.WriteSize();
		//header->id = 1;


		//sendBuffer->Close(bw.WriteSize());

		vector<BuffData> buffs{ BuffData{100, 1.5f},BuffData{200, 2.3f}, BuffData{300, 0.7f} };

		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_Test(1001, 100, 10, buffs);

		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();

}