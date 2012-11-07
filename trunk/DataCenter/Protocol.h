/*
 * Protocol.h
 *
 *  Created on: Nov 6, 2012
 *      Author: mxx
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_
#include "Packet.h"
class Protocol
{
public:
	Protocol();
	virtual ~Protocol();

	Packet& Response(Packet& inPacket);
protected:
	Packet responsePacket;
};

#endif /* PROTOCOL_H_ */
