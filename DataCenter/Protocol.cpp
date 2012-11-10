/*
 * Protocol.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: mxx
 */

#include "Protocol.h"
#include "RegistPacket.h"
#include "DataStore.h"
#include <string>

using namespace std;

Protocol::Protocol()
{
	tResponseTimeOut = 10;
	nResponseTimeOutCount = 0;
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
		bResponsed = onRegist(inPacket);
		break;
	case Packet::TERMINAL_REGIST:
		//onRegist;
		break;
	default:
		break;
	}
	return bResponsed;
}

bool Protocol::onRegist(Packet& inPacket)
{
	RegistPacket Register(inPacket);
	DataStore data;
	string& strVIN = Register.GetTerminalVIN();
	if (data.FindTerminalRecord(strVIN.c_str()))
	{

	}
	return false;
}
