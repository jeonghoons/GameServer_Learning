#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include "DBConnectionPool.h"

#include "SocketUtils.h"
#include "Listener.h"

int main()
{
	ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={ODBC Driver 17 for SQL Server};Server=(localdb)\\MSSQLLocalDB;Database=ServerDB;Trusted_Connection=Yes;"));

	// Creat Table
	{
		auto query = L"									\
			DROP TABLE IF EXISTS [dbo].[Gold];			\
			CREATE TABLE [dbo].[Gold]					\
			(											\
				[id] INT NOT NULL PRIMARY KEY IDENTITY, \
				[gold] INT NULL							\
			);";

		DBConnection* dbConn = GDBConnectionPool->Pop();
		ASSERT_CRASH(dbConn->Execute(query));
		GDBConnectionPool->Push(dbConn);
	}

	// 데이터 저장
	for (int32 i = 0; i < 3; ++i)
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();
		dbConn->Unbind();

		int32 gold = 50;
		SQLLEN len = 0;

		ASSERT_CRASH(dbConn->BindParam(1, SQL_C_LONG, SQL_INTEGER, sizeof(gold), &gold, &len));

		ASSERT_CRASH(dbConn->Execute(L"INSERT INTO [dbo].[Gold]([gold]) VALUES(?)"));
		GDBConnectionPool->Push(dbConn);
	}

	// 데이터 Read
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();
		dbConn->Unbind();

		int32 gold = 50;
		SQLLEN len = 0;
		ASSERT_CRASH(dbConn->BindParam(1, SQL_C_LONG, SQL_INTEGER, sizeof(gold), &gold, &len));



		int32 outId = 0;
		SQLLEN outIdlen = 0;
		dbConn->BindCol(1, SQL_C_LONG, sizeof(outId), &outId, &outIdlen);

		int32 outGold = 0;
		SQLLEN outGoldlen = 0;
		dbConn->BindCol(2, SQL_C_LONG, sizeof(outGold), &outGold, &outGoldlen);


		ASSERT_CRASH(dbConn->Execute(L"SELECT id, gold FROM [dbo].[Gold] WHERE gold = (?)"));
	
		while (dbConn->Fetch())
		{
			cout << "Id: " << outId << " Gold: " << outGold << endl;
		}

		GDBConnectionPool->Push(dbConn);
	}


	




	ListenerRef listener = MakeShared<Listener>();
	listener->StartAccept(NetAddress(L"127.0.0.1", 7777));

	for (int32 i = 0; i < 5; ++i)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					GIocpCore.Dispatch();
				}
			});
	}

	GThreadManager->Join();

}