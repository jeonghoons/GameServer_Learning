#include "pch.h"
#include "ThreadManager.h"
#include "Session.h"
#include "Service.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ServerPacketHandler.h"
#include <tchar.h>
#include "Protocol.pb.h"

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
	// char sendData2[1000] = u8"가"; // UTF8 = Unicode (한글 3바이트 + 로마 1바이트)
	WCHAR sendData3[1000] = L"가"; // UTF16 = Unicode (한글/로마 2바이트)
	TCHAR sendData4[1000] = _T("가");

	char sendData[1000] = "Hello World";


	while (true)
	{
		Protocol::S_TEST pkt;
		pkt.set_id(1000);
		pkt.set_hp(100);
		pkt.set_attack(10);

		{
			Protocol::BuffData* data = pkt.add_buffs();
			data->set_buffid(100);
			data->set_remaintime(1.2f);
			data->add_victims(4000);
		}

		{
			Protocol::BuffData* data = pkt.add_buffs();
			data->set_buffid(200);
			data->set_remaintime(2.5f);
			data->add_victims(1000);
			data->add_victims(2000);
		}
		

		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();

}