/*
 * Packet.h
 *
 *  Created on: Nov 6, 2012
 *      Author: mxx
 */

#ifndef PACKET_H_
#define PACKET_H_
#include <string>
#include <arpa/inet.h>

using namespace std;

#define FETCH_BYTES(x,y)   x.assign((const char*)y,sizeof(y))

class Packet
{
public:
	typedef enum
	{
		TERMINAL_GENERAL_RESPONS = 0x0001,
		TERMINAL_GENERAL_HEARTBEAT,
		TERMINAL_REGIST = 0x0100,
		TERMINAL_REGIST_RESPONSE = 0x8100,
		PLATEFORM_GENERAL_RESPONS = 0x8001,
		MESSAGE_UNKNOWN
	} MESSAGE_ID;

	typedef unsigned short WORD;
	typedef unsigned char BYTE;
	typedef unsigned int DWORD;
	typedef unsigned char BCD;

	Packet();

	Packet(MESSAGE_ID id, const char* szMobileNumber, string& strBody)
	{
		m_ID = id;
		m_strMoblieNumber = szMobileNumber;
		m_nSerialNumber = MessageSerialNumber++;
		m_strBody = strBody;
	}
	;

	virtual ~Packet();



	typedef struct _TerminalParameterItem
	{
		DWORD ParameterID;
		BYTE Length;
		BYTE Value[1];
	}__attribute__ ((packed)) TerminalParameterItem;

	void virtual Clear(void)
	{
		m_strMoblieNumber.clear();
		m_ID = MESSAGE_UNKNOWN;
		strPacketBuffer.clear();
	}
	;
	string& PackMessage(string& strBody);
	bool Parse(string& strRawData);

	MESSAGE_ID GetMessageID(void)
	{
		return m_ID;
	}
	;
	WORD GetSerialNumber(void)
	{
		return m_nSerialNumber;
	}
	;
	string& GetMobileNumber(void)
	{
		return m_strMoblieNumber;
	}
	;
	void SetMessageID(MESSAGE_ID id)
	{
		m_ID = id;
	}
	;
	void virtual Dump(void);
protected:
	MESSAGE_ID m_ID;

	enum PACKETID
	{
		ID = 0x7e, ESC = 0x7d
	};

	enum CRYPTE_DEF
	{
		PLAIN = 0, RSA
	};
	//FIXME: only work for little endian system , need modify for big endian
	typedef union _MessageProp
	{
		struct
		{
			WORD length :10;
			WORD encrypt :3;
			WORD fragment :1;
			WORD reserved :2;
		} value;
		WORD dummy;
	}__attribute__ ((packed)) MessageProp;

	typedef struct _MessageHead
	{
		WORD MsgID;
		MessageProp MsgProperty;
		BCD MobileTelNo[6];
		WORD SerialNo;
	}__attribute__ ((packed)) MessageHead;

	typedef struct _MessagePack
	{
		WORD PackSum;
		WORD PackSN;
	}__attribute__ ((packed)) MessagePack;

	static WORD MessageSerialNumber;
	unsigned int m_nSerialNumber;
	string m_strMoblieNumber;
	string m_strBody;

	void setHeadMobile(MessageHead& head);
	void packHead(MessageHead& head);
	BYTE checkSum(string& str);
	void transformRcv(string& str);
	void transformSnd(string& str);
	bool verifyCheckSum();
	void bcd2string(string& str, BCD* bcd);

	string strPacketBuffer;

};

#endif /* PACKET_H_ */
