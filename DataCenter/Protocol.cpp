/*
 * Protocol.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: mxx
 */

#include "Protocol.h"

Protocol::Protocol()
{
	// TODO Auto-generated constructor stub

}

Protocol::~Protocol()
{
	// TODO Auto-generated destructor stub
}

Packet& Protocol::Response(Packet& inPacket)
{
	switch(inPacket.GetMessageID())
	{
	case Packet::MESSAGE_UNKNOWN:

		break;
	case Packet::TERMINAL_REGIST:
		break;
	}
	return responsePacket;
}
