/*
 * RegistResponsePacket.h
 *
 *  Created on: Nov 10, 2012
 *      Author: mxx
 */

#ifndef REGISTRESPONSEPACKET_H_
#define REGISTRESPONSEPACKET_H_

#include "Packet.h"

class RegistResponsePacket: public virtual Packet
{
public:
	typedef struct _TerminalRegisteResponse
	{
		WORD SerialNo;
		BYTE Result;
		BYTE AuthCode[1];
	}__attribute__ ((packed)) TerminalRegisteResponse;

	typedef enum _REGIST_RESULT_CODE
	{
		REGISTE_OK = 0,
		DUP_REGISTED_VEHICLE,
		NO_RECORED_VEHICLE,
		DUP_REGISTED_TERMINAL,
		NO_RECORDER_TERMINAL
	} REGIST_RESULT_CODE;

	RegistResponsePacket();
	RegistResponsePacket(int SerialNo, const char* szMobileNumber,
			REGIST_RESULT_CODE result, const char* szAuthSting);
	virtual ~RegistResponsePacket();

protected:
	TerminalRegisteResponse response;
};

#endif /* REGISTRESPONSEPACKET_H_ */
