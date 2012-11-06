/*
 * Packet.h
 *
 *  Created on: Nov 6, 2012
 *      Author: mxx
 */

#ifndef PACKET_H_
#define PACKET_H_
#include <string>
using namespace std;
class Packet
{
public:
	typedef enum
	{
		TERMAINL_GENERAL_RESPONS = 0x0001,
		TERMINAL_GENERAL_HEARTBEAT,
		TERMINAL_REGIST = 0x0100,
		MESSAGE_UNKNOWN
	} MESSAGE_ID;

	Packet();
	Packet(MESSAGE_ID id, const char* szMobileNumber)
	{
		m_ID = id; m_strMoblieNumber = szMobileNumber;
		m_nSerialNumber = 0;
	}
	virtual ~Packet();



	string& PackMessage(string& strBody,string& strPackBuffer);
	MESSAGE_ID UnPackMessage(string& strRawData);
	string m_strMoblieNumber;
	string m_strBody;
	unsigned int m_nSerialNumber;
	MESSAGE_ID m_ID;

protected:

	enum PACKETID
	{
		ID = 0x7e,
		ESC =0x7d
	};

	typedef unsigned short WORD;
	typedef unsigned char BYTE;
	typedef unsigned int DWORD;
	typedef unsigned char BCD;

	//FIXME: only work for little endian system , need modify for big endian
	typedef struct _MessageProp
	{
		WORD length:10;
		WORD encrypt:3;
		WORD fragment:1;
		WORD reserved:2;
	} MessageProp;

	typedef struct _MessageHead
	{
		WORD MsgID;
		MessageProp MsgProperty;
		BCD MobileTelNo[6];
		WORD SerialNo;
	} MessageHead;

	typedef struct _MessagePack
	{
		WORD PackSum;
		WORD PackSN;
	} MessagePack;

	static WORD MessageSerialNumber;

	void setHeadMobile(MessageHead& head);
	void packHead(MessageHead& head);
	BYTE checkSum(string& str);
};

#endif /* PACKET_H_ */
