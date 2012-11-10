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
		DataStore::Vehicle_Record record;
		record.Manufacture = Register.GetManufacture();
		record.TerminalID = Register.GetTerminalID();
		record.TerminalType = Register.GetType();
		record.VechicleID = Register.GetTerminalVIN();
		record.cTermialColor = Register.GetColor();
		record.nProvince = Register.GetProvinceCode();
		record.nCity = Register.GetCityCode();
		data.UpdateTerminalRecord(record);
	}

	return true;
}
