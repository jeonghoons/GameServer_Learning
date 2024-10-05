#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Room.h"
#include "GameSession.h"


PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);

	// DB 처리후 메모리에 저장

	static Atomic<uint64> idGenerator = 1;

	{
		auto player = loginPkt.add_players();
		u8string str{ u8"DB에서 긁어온 이름2" };
		player->set_name(std::string(str.begin(), str.end()));
		player->set_playertype(Protocol::PLAYER_TYPE_1);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->tpye = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->_players.push_back(playerRef);
	}

	{
		auto player = loginPkt.add_players();
		u8string str{ u8"DB에서 긁어온 이름2" };
		player->set_name(std::string(str.begin(), str.end()));
		player->set_playertype(Protocol::PLAYER_TYPE_2);

		PlayerRef playerRef = MakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->tpye = player->playertype();
		playerRef->ownerSession = gameSession;

		gameSession->_players.push_back(playerRef);
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);


	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	uint64 index = pkt.playerindex();

	PlayerRef player = gameSession->_players[index];
	
	GRoom->DoAsync(&Room::Enter, player);

	Protocol::S_ENTER_GAME enterGamepkt;
	enterGamepkt.set_success(true);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamepkt);
	player->ownerSession->Send(sendBuffer);


	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	std::cout << pkt.msg() << endl;

	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg(pkt.msg());
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	GRoom->Broadcast(sendBuffer);

	return true;
}




