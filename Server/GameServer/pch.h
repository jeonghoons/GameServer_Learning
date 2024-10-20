#pragma once

// 여기에 미리 컴파일하려는 헤더 추가
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif // _DEBUG


#include "CorePch.h"

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"
#include "ClientPacketHandler.h"
#include "Utils.h"
#include "GameSession.h"

using GameSessionRef = shared_ptr<class GameSession>;
using PlayerRef = shared_ptr<class Player>;
using RoomRef = shared_ptr<class Room>;


