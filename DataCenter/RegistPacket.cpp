/*
 * RegistPacket.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: mxx
 */

#include "RegistPacket.h"

RegistPacket::RegistPacket()
{
}

RegistPacket::~RegistPacket()
{
}

RegistPacket::RegistPacket(Packet& packet):Packet(packet)
{
	if (packet.GetMessageID() != TERMINAL_REGIST)
		m_strBody.clear();
	return;
}

string& RegistPacket::GetTerminalVIN()
{
	TerminalRegiste* info;
	info = (TerminalRegiste*)m_strBody.data();
	strVIN.clear();
	if (m_strBody.size() > sizeof(TerminalRegiste))
	   strVIN = string((const char*)info->VIN, m_strBody.size() - sizeof(TerminalRegiste));
	return strVIN;
}
