#include "pch.h"
#include "ThreadManager.h"
#include "Session.h"
#include "Service.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include "Room.h"
#include <tchar.h>
#include "Protocol.pb.h"

enum {WORKER_TICK = 64};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		service->GetIocpCore()->Dispatch(10);

		ThreadManager::DistributeReservedJobs();

		ThreadManager::DoGlobalQueueWork();
	}
}

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
	GRoom->DoTimer(2000, [] {cout << "Hello 2000" << endl; });
	GRoom->DoTimer(3000, [] {cout << "Hello 3000" << endl; });
	GRoom->DoTimer(4000, [] {cout << "Hello 4000" << endl; });

	ClientPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>,
		100);

	ASSERT_CRASH(service->Start());

	/*for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}*/

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
		{
			DoWorkerJob(service);
		});
	}

	

	GThreadManager->Join();

}