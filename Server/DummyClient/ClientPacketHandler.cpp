#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"

void ClientPacketHandler::HandlerPacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;
	// cout << "Packet ID : " << header.id << " , Size : " << header.size << endl;

	switch (header.id)
	{
	case S_TEST:
		Handle_S_TEST(buffer, len);
		break;
	default:
		break;
	}

}

void ClientPacketHandler::Handle_S_TEST(BYTE* buffer, int32 len)
{
	Protocol::S_TEST pkt;

	ASSERT_CRASH(pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)));

	cout << pkt.id() << " " << pkt.hp() << " " << pkt.attack() << endl;

	cout << "BUFSIZE : " << pkt.buffs_size() << endl;

	for (auto& buf : pkt.buffs())
	{
		cout << "BUF INFO : " << buf.buffid() << ", " << buf.remaintime() << endl;
		cout << "VICTIMS : " << buf.victims_size() << endl;
		for (auto& vic : buf.victims())
		{
			cout << vic << " ";
		}
		cout << endl;
	}
}


//#pragma pack(1)
//struct PKT_S_TEST
//{
//	struct BuffsListItem
//	{
//		uint64 buffId;
//		float remainTime;
//
//		uint16 victimsOffset;
//		uint16 victimsCount;
//
//		bool Validate(BYTE* packetStart, uint16 packetSize, uint32& size)
//		{
//			if (victimsOffset + victimsCount * sizeof(uint64) > packetSize)
//				return false;
//
//			size += victimsCount * sizeof(uint64);
//			return true;
//		}
//	};
//
//	uint16 packetSize; // 공용 헤더
//	uint16 packetId; // 공용 헤더
//	uint64 id;
//	uint32 hp;
//	uint16 attack;
//
//	uint16 buffsOffset;
//	uint16 buffsCount;
//
//	bool Validate()
//	{
//		uint32 size = 0;
//
//		size += sizeof(PKT_S_TEST);
//		if (packetSize < size)
//			return false;
//
//		if (buffsOffset + buffsCount * sizeof(BuffsListItem) > packetSize)
//			return false;
//
//		// 가변 데이터 그기 추가
//		size += buffsCount * sizeof(BuffsListItem);
//		BuffsList buffList = GetBuffsList();
//		for (int32 i = 0; i < buffList.Count(); i++)
//		{
//			if (buffList[i].Validate((BYTE*)this, packetSize, size) == false)
//				return false;
//		}
//
//		// 최종 패킷 크기 확인
//		if (size != packetSize)
//			return false;
//
//		
//
//		return true;
//	}
//
//	using BuffsList = PacketList<PKT_S_TEST::BuffsListItem>;
//	using BuffsVictimsList = PacketList<uint64>;
//
//	BuffsList GetBuffsList()
//	{
//		BYTE* data = reinterpret_cast<BYTE*>(this);
//		data += buffsOffset;
//		return BuffsList(reinterpret_cast<PKT_S_TEST::BuffsListItem*>(data), buffsCount);
//	}
//
//	BuffsVictimsList GetBuffsVictimList(BuffsListItem* buffsItem)
//	{
//		BYTE* data = reinterpret_cast<BYTE*>(this);
//		data += buffsItem->victimsOffset;
//		return BuffsVictimsList(reinterpret_cast<uint64*>(data), buffsItem->victimsCount);
//	}
//
//
//	// vector<int64> buffs; // 가변 데이터
//	// wstring name;
//};
//#pragma pack()



