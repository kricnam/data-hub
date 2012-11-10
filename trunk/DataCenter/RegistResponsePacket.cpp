/*
 * RegistResponsePacket.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: mxx
 */

#include "RegistResponsePacket.h"
#include <arpa/inet.h>

RegistResponsePacket::RegistResponsePacket()
{
	m_ID = TERMINAL_REGIST_RESPONSE;
}

RegistResponsePacket::RegistResponsePacket(int SerialNo,
		const char* szMobileNumber, REGIST_RESULT_CODE result,
		const char* szAuthSting)
{
	m_ID = TERMINAL_REGIST_RESPONSE;
	m_strMoblieNumber = szMobileNumber;
	response.SerialNo = htons(SerialNo);
	response.Result = result;
	m_strBody.assign((const char*) &response,
			sizeof(response) - sizeof(response.AuthCode));
	m_strBody += szAuthSting;
}

RegistResponsePacket::~RegistResponsePacket()
{

}

