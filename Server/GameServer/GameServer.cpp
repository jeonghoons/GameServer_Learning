#include "pch.h"
#include "ThreadManager.h"
#include "Session.h"
#include "Service.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ServerPacketHandler.h"
#include <tchar.h>

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

	
	char sendData1[1000] = "가"; // CP-949 = KS-X-1001(한글2바이트) + KS-X-1003(로마1바이트)
	char sendData2[1000] = u8"가"; // UTF8 = Unicode (한글 3바이트 + 로마 1바이트)
	WCHAR sendData3[1000] = L"가"; // UTF16 = Unicode (한글/로마 2바이트)
	TCHAR sendData4[1000] = _T("가");
	
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

		PKT_S_TEST_WRITE pktWriter(1001, 100, 10);
		PKT_S_TEST_WRITE::BuffsList buffList = pktWriter.ReserveBuffsList(3);
		buffList[0] = { 100, 1.5 };
		buffList[1] = { 200, 2.3 };
		buffList[2] = { 300, 0.7 };

		PKT_S_TEST_WRITE::BuffsVictimsList vic0 = pktWriter.ReserveBuffsVictimsList(&buffList[0], 3);
		{
			vic0[0] = 1000;
			vic0[1] = 2000;
			vic0[2] = 3000;
		}

		PKT_S_TEST_WRITE::BuffsVictimsList vic1 = pktWriter.ReserveBuffsVictimsList(&buffList[1], 1);
		{
			vic1[0] = 1000;
		}

		PKT_S_TEST_WRITE::BuffsVictimsList vic2 = pktWriter.ReserveBuffsVictimsList(&buffList[2], 2);
		{
			vic2[0] = 3000;
			vic2[1] = 5000;
		}
		
		SendBufferRef sendBuffer = pktWriter.CloseAndReturn();

		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();

}