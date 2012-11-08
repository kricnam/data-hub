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
class Protocol
{
public:
	Protocol();
	virtual ~Protocol();

	bool Response(Packet& inPacket);
protected:
	PacketQueue outQueue;
	Packet responsePacket;
};

#endif /* PROTOCOL_H_ */
