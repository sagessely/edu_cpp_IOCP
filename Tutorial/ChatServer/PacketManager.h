#pragma once

#include "Packet.h"

#include <unordered_map>
#include <deque>
#include <functional>
#include <thread>
#include <mutex>


class UserManager;
class RoomManager;
class RedisManager;

class PacketManager {
public:
	PacketManager() = default;
	~PacketManager() = default;

	void Init(const UINT32 maxClient);

	bool Run();

	void End();

	void ReceivePacketData(const UINT32 clientIndex_, const UINT32 size_, char* pData_);

	void PushSystemPacket(PacketInfo packet);
		
	std::function<void(UINT32, UINT32, char*)> SendPacketFunc;


private:
	void CreateCompent(const UINT32 maxClient);

	void ProcessPacket();

	void ProcessRecvPacket(const UINT32 connectionIndex_, const UINT16 packetId_, const UINT16 packetSize_, char* pPacket_);

	void ProcessUserConnect(UINT32 connIndex, UINT16 packetSize_, char* pPacket_);
	void ProcessUserDisConnect(UINT32 connIndex, UINT16 packetSize_, char* pPacket_);
	void ProcessLogin(UINT32 connIndex, UINT16 packetSize_, char* pPacket_);
	void ProcessEnterRoom(UINT32 connIndex, UINT16 packetSize_, char* pPacket_);
	void ProcessLeaveRoom(UINT32 connIndex, UINT16 packetSize_, char* pPacket_);
	void ProcessRoomChatMessage(UINT32 connIndex, UINT16 packetSize_, char* pPacket_);

	void ClearConnectionInfo(INT32 connIndex);

	void EnqueuePacketData(const UINT32 sessionIndex);
	PacketInfo DequePacketData();

	PacketInfo DequeSystemPacketData();


	typedef void(PacketManager::* PROCESS_RECV_PACKET_FUNCTION)(UINT32, UINT16, char*);
	std::unordered_map<int, PROCESS_RECV_PACKET_FUNCTION> m_RecvFuntionDictionary;

	UserManager* m_pUserManager;
	RoomManager* m_pRoomManager;	
	RedisManager* mRedisMgr;
		
	std::function<void(int, char*)> m_SendMQDataFunc;


	bool mIsRunProcessThread = false;
	
	std::thread mProcessThread;
	
	std::mutex mLock;
	
	std::deque<UINT32> mInComingPacketUserIndex;

	std::deque<PacketInfo> mSystemPacketQueue;
};

