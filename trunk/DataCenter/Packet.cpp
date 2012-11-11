/*
 * Packet.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: mxx
 */

#include "Packet.h"
#include <string.h>
#include <arpa/inet.h>
#include "TraceLog.h"

Packet::WORD Packet::MessageSerialNumber = 0;

Packet::Packet()
{
	m_ID = MESSAGE_UNKNOWN;
	m_nSerialNumber = 0;
	m_bCheckSumError = false;
}

Packet::~Packet()
{

}

string& Packet::PackMessage(string& strBody)
{
	MessageHead head;
	strPacketBuffer.clear();
	head.MsgID = m_ID;
	head.SerialNo = m_nSerialNumber;
	setHeadMobile(head);
	head.MsgProperty.dummy = 0;
	head.MsgProperty.value.length = strBody.size();
	packHead(head);
	strPacketBuffer.assign((const char*) &head, sizeof(head));
	strPacketBuffer.append(strBody);
	strPacketBuffer.append(1, checkSum(strPacketBuffer));
	transformSnd(strPacketBuffer);
	return strPacketBuffer;
}
// extract the frame from streams, return true when frame is extracted
// no matter what  the checksum or structure is correct
bool Packet::Parse(string& strRawData)
{
	m_ID = MESSAGE_UNKNOWN;
	string::size_type frameStart = strRawData.find(ID);
	if (frameStart != string::npos)
	{
		string::size_type frameEnd = strRawData.find(ID, frameStart + 1);
		if (frameEnd != string::npos)
		{
			strPacketBuffer = strRawData.substr(frameStart + 1,
					frameEnd - frameStart - 1);
			strRawData.erase(frameStart, frameEnd - frameStart + 1);
			transformRcv(strPacketBuffer);
			if (verifyCheckSum())
			{
				MessageHead Head = *((MessageHead*) strPacketBuffer.data());
				m_ID = (MESSAGE_ID) ntohs(Head.MsgID);
				Head.MsgProperty.dummy = ntohs(Head.MsgProperty.dummy);
				m_nSerialNumber = ntohs(Head.SerialNo);
				bcd2string(m_strMoblieNumber, Head.MobileTelNo);
				m_strBody = strPacketBuffer.substr(sizeof(MessageHead) - 1,
						strPacketBuffer.size() - 1 - sizeof(MessageHead));
				TRACE(
						"Head.Length=%d @ %d", Head.MsgProperty.value.length, m_strBody.size());
				m_bCheckSumError = (Head.MsgProperty.value.length != m_strBody.size());
				strPacketBuffer.clear();
			}
			else
				m_bCheckSumError = true;

			return true;
		}
	}
	return false;
}

void Packet::setHeadMobile(MessageHead& head)
{
	string::reverse_iterator rit;
	memset(head.MobileTelNo, 0, sizeof(head.MobileTelNo));
	rit = m_strMoblieNumber.rbegin();
	for (int i = sizeof(head.MobileTelNo); i > 0; i--)
	{
		if (rit == m_strMoblieNumber.rend())
			break;
		head.MobileTelNo[i - 1] = ((*rit) - 0x30);
		rit--;
		if (rit == m_strMoblieNumber.rend())
			break;
		head.MobileTelNo[i - 1] |= 0xF0 & (((*rit) - 0x30) << 4);
	}
}

void Packet::packHead(MessageHead& head)
{
	head.MsgID = htons(head.MsgID);
	head.MsgProperty.dummy = htons(head.MsgProperty.dummy);
	head.SerialNo = htons(head.SerialNo);
}

Packet::BYTE Packet::checkSum(string& str)
{
	string::iterator it;
	BYTE sum = 0;
	for (it = str.begin(); it != str.end(); it++)
	{
		if (it == str.begin())
			sum = (*it);
		else
			sum ^= (*it);
	}
	return sum;
}

void Packet::transformRcv(string& str)
{
	string::size_type pos = str.find(ESC);
	string strTmp;
	while (pos != string::npos)
	{
		strTmp += str.substr(0, pos);
		if (str.at(pos + 1) == 0x01)
			strTmp += ESC;
		else if (str.at(pos + 1) == 0x02)
			strTmp += ID;
		//else exception;
		str.erase(0, pos + 2);
		pos = str.find(ESC);
	};
	if (strTmp.size())
		str = strTmp;
	return;
}

void Packet::transformSnd(string& str)
{
	string::iterator it;
	string strTmp;
	for (it = str.begin(); it != str.end(); it++)
	{
		if ((*it) == ID)
		{
			strTmp.append(1, ESC).append(1, 0x02);
			continue;
		}
		if ((*it) == ESC)
		{
			strTmp.append(1, ESC).append(1, 0x01);
			continue;
		}
		strTmp += (*it);
	}
	str = strTmp;
	return;
}

bool Packet::verifyCheckSum()
{
	return checkSum(strPacketBuffer) == 0;
}

void Packet::Dump(void)
{
	DEBUG("%u:size=%u", m_ID, m_nSerialNumber, m_strBody.size());
}

void Packet::bcd2string(string& str, BCD* bcd)
{
	for (int i = 0; i < 6; i++)
	{
		str += ((bcd[i] & 0xF0) >> 4) + 0x30;
		str += ((bcd[i] & 0x0F)) + 0x30;
	}
}
