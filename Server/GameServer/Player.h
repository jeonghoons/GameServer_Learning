#pragma once
class Player
{
public:
	uint64						playerId = 0;
	string						name;
	Protocol::PlayerType			tpye = Protocol::PLAYER_TYPE_NONE;
	GameSessionRef				ownerSession;
};

