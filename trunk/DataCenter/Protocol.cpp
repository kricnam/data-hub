/*
 * Protocol.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: mxx
 */

#include "Protocol.h"
#include "RegistPacket.h"
#include "RegistResponsePacket.h"
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
	RegistResponsePacket::REGIST_RESULT_CODE result = RegistResponsePacket::REGISTE_OK;
	DataStore::Vehicle_Record record;
	do
	{
		if (!data.FindTerminalRecord(Register.GetTerminalID().c_str()))
		{
			result = RegistResponsePacket::NO_RECORED_VEHICLE;
			break;
		}

		if (!data.FindVechileRecord(Register.GetTerminalVIN().c_str()))
		{
			result = RegistResponsePacket::NO_RECORDER_TERMINAL;
			break;
		}

		if (data.GetVechicleRecord(record))
		{
			if (record.TerminalID.empty())
			{
				record.Manufacture = Register.GetManufacture();
				record.TerminalID = Register.GetTerminalID();
				record.TerminalType = Register.GetType();
				record.VechicleID = Register.GetTerminalVIN();
				record.cTermialColor = Register.GetColor();
				record.nProvince = Register.GetProvinceCode();
				record.nCity = Register.GetCityCode();
				if (!data.RegisteTerminal(record))
					result = RegistResponsePacket::DUP_REGISTED_TERMINAL;
			}
			else
				result = RegistResponsePacket::DUP_REGISTED_VEHICLE;
		}

	} while (false);

	RegistResponsePacket response(inPacket.GetSerialNumber(),
			inPacket.GetMobileNumber().c_str(),result,record.strAuthCode.c_str());

	outQueue.Push(response);
	return true;
}
