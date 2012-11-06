/*
 * Packet.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: mxx
 */

#include "Packet.h"
#include <string.h>
#include <arpa/inet.h>

Packet::WORD Packet::MessageSerialNumber = 0;

Packet::Packet()
{
	// TODO Auto-generated constructor stub

}

Packet::~Packet()
{
	// TODO Auto-generated destructor stub
}

string& Packet::PackMessage(string& strBody,string& strPackBuffer)
{
	strPackBuffer.clear();
	MessageHead head;
	head.MsgID = m_ID;
	head.SerialNo = MessageSerialNumber++;
	setHeadMobile(head);
	head.MsgProperty.length = strBody.size();
	packHead(head);
	strPackBuffer.append((const char*)&head,sizeof(head));
	strPackBuffer.append(strBody);
	strPackBuffer.append(1,checkSum(strPackBuffer));
	return strPackBuffer;
}

Packet::MESSAGE_ID Packet::UnPackMessage(string& strRawData)
{
	return MESSAGE_UNKNOWN;
}

void Packet::setHeadMobile(MessageHead& head)
{
	string::reverse_iterator rit;
	memset(head.MobileTelNo,0,sizeof(head.MobileTelNo));
	rit = m_strMoblieNumber.rbegin();
	for(int i=sizeof(head.MobileTelNo);i > 0 ; i--)
	{
		if (rit == m_strMoblieNumber.rend()) break;
		head.MobileTelNo[i-1] = ((*rit) - 0x30);
		rit--;
		if (rit == m_strMoblieNumber.rend()) break;
		head.MobileTelNo[i-1] |= 0xF0 & (((*rit) - 0x30)<<4);
	}
}

void Packet::packHead(MessageHead& head)
{
	head.MsgID = htons(head.MsgID);
	WORD n =htons(*(WORD*)&head.MsgProperty);
	head.MsgProperty = *(MessageProp*)&n;
	head.SerialNo = htons(head.SerialNo);
}

Packet::BYTE Packet::checkSum(string& str)
{
	string::iterator it;
	BYTE sum = 0;
	for(it = str.begin();it != str.end(); it++)
	{
		if (it == str.begin()) sum = (*it);
		else sum ^= (*it);
	}
	return sum;
}
