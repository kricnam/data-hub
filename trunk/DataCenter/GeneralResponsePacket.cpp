/*
 * GeneralResponsePacket.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: mxx
 */

#include "GeneralResponsePacket.h"

GeneralResponsePacket::GeneralResponsePacket()
{
	m_ID = PLATEFORM_GENERAL_RESPONS;
	m_bResponse = true;
}

GeneralResponsePacket::~GeneralResponsePacket()
{

}

GeneralResponsePacket::GeneralResponsePacket(Packet& packet):Packet(packet)
{

}

GeneralResponsePacket::GeneralResponsePacket(Packet& packet,
		GENERAL_RESULT_CODE result):Packet(packet)
{
	m_ID = PLATEFORM_GENERAL_RESPONS;
	m_bResponse = true;
	SetResponse(packet.GetSerialNumber(),packet.GetMessageID(),result);
}

void GeneralResponsePacket::SetResponse(WORD nSerialNo,MESSAGE_ID message,
		GENERAL_RESULT_CODE result)
{
	GENERALResponse response;
	response.ResponsSN = htons(nSerialNo);
	response.ResponsID = htons(message);
	response.Result = result;
	m_strBody.assign((const char*)&response,sizeof(response));
}

GeneralResponsePacket::GENERAL_RESULT_CODE GeneralResponsePacket::GetResponeResult(WORD& onSerialNo,
		MESSAGE_ID& onMessage)
{
	GENERALResponse* ptrR;
	ptrR = (GENERALResponse*)m_strBody.data();
	onSerialNo = ntohs(ptrR->ResponsSN);
	onMessage =(MESSAGE_ID) ntohs(ptrR->ResponsID);
	return (GENERAL_RESULT_CODE)ptrR->Result;
}
