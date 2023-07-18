#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave()
{
	// user lock
	lock_guard<mutex> guard(_mutex);


	// account lock
	Account* account = AccountManager::Instance()->GetAccount(100);

}
