/*
 * GeneralResponsePacket.h
 *
 *  Created on: Nov 10, 2012
 *      Author: mxx
 */

#ifndef GENERALRESPONSEPACKET_H_
#define GENERALRESPONSEPACKET_H_

#include "Packet.h"

class GeneralResponsePacket: public virtual Packet
{
public:
	GeneralResponsePacket();
	virtual ~GeneralResponsePacket();

	typedef enum _GENERAL_RESULT_CODE
	{
		SUCCESS = 0, FAIL, ERROR_MESSAGE, NO_SUPPORTED, ALARM_AFIMITIV
	} GENERAL_RESULT_CODE;

	typedef struct _GENERALResponse
	{
		WORD ResponsSN;
		WORD ResponsID;
		BYTE Result;
	}__attribute__ ((packed)) GENERALResponse;

	GeneralResponsePacket(Packet& packet);
	GeneralResponsePacket(Packet& packet,GENERAL_RESULT_CODE result);
	void SetResponse(WORD nSerialNo, MESSAGE_ID message, GENERAL_RESULT_CODE result);
};

#endif /* GENERALRESPONSEPACKET_H_ */
