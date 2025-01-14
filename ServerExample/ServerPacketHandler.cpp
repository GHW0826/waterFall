#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h""

void ServerPacketHandler::HandlePacket(BYTE* buffer, int32 len) {
	BufferReader br(buffer, len);
	PacketHeader header;
	br.Peek(&header);

	switch (header.id) {
	default:
		break;
	}
}



SendBufferRef ServerPacketHandler::Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs) {
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096);

	BufferWriter bw(sendBuffer->Buffer(), 4096);
	PacketHeader* header = bw.Reserve<PacketHeader>();

	BYTE* buffer = sendBuffer->Buffer();
	// id (uint64), 체력 (uint32), 공격력(uint16)
	bw << id << hp << attack;
	bw << (uint16)buffs.size();
	for (BuffData& buff : buffs) {
		bw << buff.buffId << buff.remainTime;
	}

	header->size = bw.WriteSize();
	header->id = S_TEST;
	sendBuffer->Close(sizeof(PacketHeader));
	return sendBuffer;
}
