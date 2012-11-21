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
	tSendTime = 0;
	nTransmitCount = 0;
	tTimeOut = 0;
	m_bResponse = false;
}

Packet::~Packet()
{

}

string& Packet::PackMessage()
{
	MessageHead head;
	if (strPacketBuffer.empty())
	{
		head.MsgID = m_ID;
		head.SerialNo = m_nSerialNumber;
		setHeadMobile(head);
		head.MsgProperty.dummy = 0;
		head.MsgProperty.value.length = m_strBody.size();
		packHead(head);
		strPacketBuffer.assign((const char*) &head, sizeof(head));
		strPacketBuffer.append(m_strBody);
		strPacketBuffer.append(1, checkSum(strPacketBuffer));
		transformSnd(strPacketBuffer);
		strPacketBuffer.insert(0, 1, (char) SYNC);
		strPacketBuffer.append(1, (char) SYNC);
	}
	return strPacketBuffer;
}
// extract the frame from streams, return true when frame is extracted
// no matter what  the checksum or structure is correct
// return true when found a data frame else return false
// if checksum error, set check sum error flag
bool Packet::Parse(string& strRawData)
{
	m_ID = MESSAGE_UNKNOWN;
	string::size_type frameStart = strRawData.find(SYNC);
	if (frameStart != string::npos)
	{
		string::size_type frameEnd = strRawData.find(SYNC, frameStart + 1);
		if (frameEnd != string::npos)
		{
			strPacketBuffer = strRawData.substr(frameStart + 1,
					frameEnd - frameStart - 1);
			strRawData.erase(frameStart, frameEnd - frameStart + 1);

			transformRcv(strPacketBuffer);

			if (verifyCheckSum())
			{
				TRACE("check sum verify ok");
				Head = *((MessageHead*) strPacketBuffer.data());
				m_ID = (MESSAGE_ID) ntohs(Head.MsgID);
				Head.MsgProperty.dummy = ntohs(Head.MsgProperty.dummy);
				m_nSerialNumber = ntohs(Head.SerialNo);
				bcd2string(m_strMoblieNumber, Head.MobileTelNo);
				//total size - header - checksum = content size
				m_strBody = strPacketBuffer.substr(sizeof(MessageHead),
						strPacketBuffer.size() - sizeof(MessageHead) - 1);
				TRACE(
						"Head.Length=%d/%d", Head.MsgProperty.value.length, m_strBody.size());

				strPacketBuffer.clear();
			}
			else
			{
				TRACE("checksum fail");
				m_bCheckSumError = true;
			}

			return true;
		}
	}
	else
		strRawData.clear(); //clear junk bytes
	return false;
}

void Packet::setHeadMobile(MessageHead& head)
{
	memset(head.MobileTelNo, 0, sizeof(head.MobileTelNo));

	string2bcd(m_strMoblieNumber, head.MobileTelNo);
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
	BYTE sum = 0x00;
	for (it = str.begin(); it != str.end(); it++)
	{
		if (it == str.begin())
			sum ^= (*it);
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
			strTmp += SYNC;
		//else exception;
		if ((pos + 2) <= str.size())
			str.erase(0, pos + 2);
		else
			break;
		pos = str.find(ESC);
	};
	if (str.size()) strTmp += str;
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
		if ((*it) == SYNC)
		{
			strTmp.append(1, (char) ESC).append(1, (char) 0x02);
			continue;
		}
		if ((*it) == ESC)
		{
			strTmp.append(1, (char) ESC).append(1, (char) 0x01);
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
	DEBUG(
			"\nID:\t0x%X\nProp:\t[0x%04X]\n\tPack=%u;Enc=%u;length=%u\nMobil:\t%s\nSN:\t0x%X", m_ID, Head.MsgProperty.dummy, Head.MsgProperty.value.fragment, Head.MsgProperty.value.encrypt, Head.MsgProperty.value.length, m_strMoblieNumber.c_str(), m_nSerialNumber);
	DEBUG("check sum:%s", m_bCheckSumError?"FAIL":"OK");

}

void Packet::bcd2string(string& str, BCD* bcd)
{
	str.clear();
	for (int i = 0; i < 6; i++)
	{
		str += ((bcd[i] & 0xF0) >> 4) + 0x30;
		str += ((bcd[i] & 0x0F)) + 0x30;
	}
}

void Packet::string2bcd(string& str, BCD* bcd)
{
	int j = 12 - str.size();
	for (int i = 0; i < 6; i++)
	{
		char c = 0;
		char d = 0;
		if ((i * 2) >= j)
		{
			c = str[i * 2 - j];
		}
		if ((i * 2 + 1) >= j)
		{
			d = str[i * 2 - j + 1];
		}
		bcd[i] = ((c & 0x0F) << 4) | (d & 0x0F);
	}
}
