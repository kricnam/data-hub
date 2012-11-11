/*
 * Protocol.h
 *
 *  Created on: Nov 6, 2012
 *      Author: mxx
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_
#include "Packet.h"
#include "PacketQueue.h"
#include "RegistPacket.h"
#include "GeneralResponsePacket.h"
#include "DataStore.h"
#include <time.h>

class Protocol
{
public:
	Protocol();
	virtual ~Protocol();

	bool Response(Packet& inPacket);
	bool m_bCloseConnect;
protected:
	typedef GeneralResponsePacket::GENERAL_RESULT_CODE RESULT;
	PacketQueue outQueue;
	time_t tResponseTimeOut;
	int nResponseTimeOutCount;
	void setRegisterRecord(RegistPacket& pack,
			DataStore::Vehicle_Record& record);
	bool generalRespons(Packet& inPacket,
			GeneralResponsePacket::GENERAL_RESULT_CODE result);
	bool bAuthorized;
	bool onRegist(Packet& inPacket);
	RESULT onAuthorize(Packet& inPacket);
	bool dispatch(Packet& inPacket);
};

#endif /* PROTOCOL_H_ */
