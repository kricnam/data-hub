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
		TERMINAL_GENERAL_RESPONS = 0x0001,
		TERMINAL_GENERAL_HEARTBEAT,
		TERMINAL_REGIST = 0x0100,
		MESSAGE_UNKNOWN
	} MESSAGE_ID;

	typedef enum _REGIST_RESULT_CODE
	{
		REGISTE_OK = 0,
		DUP_REGISTED_VEHICLE,
		NO_RECORED_VEHICLE,
		DUP_REGISTED_TERMINAL,
		NO_RECORDER_TERMINAL
	} REGIST_RESULT_CODE;

	typedef enum _GENERAL_RESULT_CODE
	{
		SUCCESS = 0,
		FAIL,
		ERROR_MESSAGE,
		NO_SUPPORTED,
		ALARM_AFIMITIV
	} GENERAL_RESULT_CODE;

	typedef unsigned short WORD;
	typedef unsigned char BYTE;
	typedef unsigned int DWORD;
	typedef unsigned char BCD;

	Packet();
	Packet(MESSAGE_ID id, const char* szMobileNumber,string& strBody)
	{
		m_ID = id; m_strMoblieNumber = szMobileNumber;
		m_nSerialNumber = 0;
		PackMessage(strBody);
	};

	virtual ~Packet();

	typedef struct _GENERALResponse
	{
		WORD ResponsSN;
		WORD ResponsID;
		BYTE Result;
	} __attribute__ ((packed)) GENERALResponse;

	typedef struct _TerminalRegiste
	{
		WORD Province;
		WORD City;
		BYTE Manufacture[5];
		BYTE TerminalType[8];
		BYTE TerminalID[7];
		BYTE BodyColor;
		BYTE VIN[1];
	} __attribute__ ((packed)) TerminalRegiste;

	typedef struct _TerminalRegisteResponse
	{
		WORD SerialNo;
		BYTE Result;
		BYTE AuthCode[1];
	} __attribute__ ((packed)) TerminalRegisteResponse;

	typedef struct _TerminalParameterItem
	{
		DWORD ParameterID;
		BYTE  Length;
		BYTE  Value[1];
	}  __attribute__ ((packed)) TerminalParameterItem ;

	void Clear(void) {m_strMoblieNumber.clear();m_ID=MESSAGE_UNKNOWN;strPacketBuffer.clear();};
	string& PackMessage(string& strBody);
	bool Parse(string& strRawData);
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

	enum CRYPTE_DEF
	{
		PLAIN = 0,
		RSA
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
	} __attribute__ ((packed)) MessageProp;

	typedef struct _MessageHead
	{
		WORD MsgID;
		MessageProp MsgProperty;
		BCD MobileTelNo[6];
		WORD SerialNo;
	} __attribute__ ((packed)) MessageHead;

	typedef struct _MessagePack
	{
		WORD PackSum;
		WORD PackSN;
	} __attribute__ ((packed)) MessagePack;

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
