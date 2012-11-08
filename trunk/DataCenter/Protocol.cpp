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

bool Protocol::Response(Packet& inPacket)
{
	bool bResponsed = false;
	switch(inPacket.GetMessageID())
	{
	case Packet::MESSAGE_UNKNOWN:
		responsePacket.Clear();
		break;
	case Packet::TERMINAL_REGIST:
		//onRegist;
		break;
	}
	return bResponsed;
}
