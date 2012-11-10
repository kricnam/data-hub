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
#include "time.h"
class Protocol
{
public:
	Protocol();
	virtual ~Protocol();

	bool Response(Packet& inPacket);
protected:
	PacketQueue outQueue;
	time_t tResponseTimeOut;
	int nResponseTimeOutCount;

	bool onRegist(Packet& inPacket);
};

#endif /* PROTOCOL_H_ */
