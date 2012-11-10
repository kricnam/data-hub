/*
 * RegistPacket.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: mxx
 */

#include "RegistPacket.h"
#include <arpa/inet.h>

RegistPacket::RegistPacket()
{
	nCity = 0;
	nProvince = 0;
	nColorCode = 0;
}

RegistPacket::~RegistPacket()
{
}

RegistPacket::RegistPacket(Packet& packet):Packet(packet)
{
	if (packet.GetMessageID() != TERMINAL_REGIST)
		m_strBody.clear();
	else
	{
		TerminalRegiste* info;
		info = (TerminalRegiste*)m_strBody.data();
		strVIN.clear();
		if (m_strBody.size() > sizeof(TerminalRegiste))
		{
			strVIN.assign((const char*)info->VIN, m_strBody.size() - sizeof(TerminalRegiste));
			nProvince = ntohs(info->Province);
			nCity = ntohs(info->City);
			FETCH_BYTES(strManufacture,info->Manufacture);
			FETCH_BYTES(strID,info->TerminalID);
			FETCH_BYTES(strType,info->TerminalType);
			nColorCode = info->BodyColor;
		}

	}
	return;
}

string& RegistPacket::GetTerminalVIN()
{
	return strVIN;
}

int RegistPacket::GetProvinceCode()
{
	return nProvince;
}

int RegistPacket::GetCityCode()
{
	return nCity;
}

string& RegistPacket::GetManufacture()
{
	return strManufacture;
}

string& RegistPacket::GetType()
{
	return strType;
}

string& RegistPacket::GetTerminalID()
{
	return strID;
}

int RegistPacket::GetColor()
{
	return nColorCode;
}
