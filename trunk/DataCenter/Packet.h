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
	Packet(MESSAGE_ID id, const char* szMobileNumber,string& strBody)
	{
		m_ID = id; m_strMoblieNumber = szMobileNumber;
		m_nSerialNumber = 0;
		PackMessage(strBody);
	};

	virtual ~Packet();

	void Clear(void) {m_strMoblieNumber.clear();m_ID=MESSAGE_UNKNOWN;strPacketBuffer.clear();};
	Packet& PackMessage(string& strBody);
	Packet& UnPackMessage(string& strRawData);
	const char* GetData(void) { return strPacketBuffer.data();};
	unsigned int GetSize(void) {return strPacketBuffer.size();};
	MESSAGE_ID GetMessageID(void) { return m_ID; };

protected:
	MESSAGE_ID m_ID;

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
