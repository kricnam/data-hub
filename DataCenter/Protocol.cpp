/*
 * Protocol.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: mxx
 */

#include "Protocol.h"
#include "RegistPacket.h"
#include "RegistResponsePacket.h"
#include "GeneralResponsePacket.h"
#include "DataStore.h"
#include <string>

using namespace std;

Protocol::Protocol()
{
	tResponseTimeOut = 10;
	nResponseTimeOutCount = 0;
	bAuthorized = false;
	m_bCloseConnect = false;
}

Protocol::~Protocol()
{
}

bool Protocol::Response(Packet& inPacket)
{
	if (inPacket.m_bCheckSumError)
	{
		return generalRespons(inPacket, GeneralResponsePacket::ERROR_MESSAGE);
	}

	if (!bAuthorized && inPacket.GetMessageID() != Packet::TERMINAL_REGIST
			&& inPacket.GetMessageID() != Packet::TERMINAL_AUTHORIZE)
	{
		m_bCloseConnect = true;
		return false;
	}

	return dispatch(inPacket);
}

void Protocol::setRegisterRecord(RegistPacket& Register,
		DataStore::Vehicle_Record& record)
{
	record.Manufacture = Register.GetManufacture();
	record.TerminalID = Register.GetTerminalID();
	record.TerminalType = Register.GetType();
	record.VechicleID = Register.GetTerminalVIN();
	record.cTermialColor = Register.GetColor();
	record.nProvince = Register.GetProvinceCode();
	record.nCity = Register.GetCityCode();
}

bool Protocol::generalRespons(Packet& inPacket,
		GeneralResponsePacket::GENERAL_RESULT_CODE result)
{
	GeneralResponsePacket respons(inPacket,
			GeneralResponsePacket::NO_SUPPORTED);
	outQueue.Push(respons);
	return true;
}

bool Protocol::onRegist(Packet& inPacket)
{
	RegistPacket Register(inPacket);
	DataStore data;
	RegistResponsePacket::REGIST_RESULT_CODE result =
			RegistResponsePacket::REGISTE_OK;
	DataStore::Vehicle_Record record;
	do
	{
		if (!data.FindTerminalRecord(Register.GetTerminalID().c_str()))
		{
			result = RegistResponsePacket::NO_RECORED_VEHICLE;
			break;
		}

		if (!data.FindVechileRecord(Register.GetTerminalVIN().c_str()))
		{
			result = RegistResponsePacket::NO_RECORDER_TERMINAL;
			break;
		}

		if (data.GetVechicleRecord(record))
		{
			if (record.TerminalID.empty())
			{
				setRegisterRecord(Register, record);
				if (!data.RegisteTerminal(record))
					result = RegistResponsePacket::DUP_REGISTED_TERMINAL;
			}
			else
				result = RegistResponsePacket::DUP_REGISTED_VEHICLE;
		}

	} while (false);

	RegistResponsePacket response(inPacket.GetSerialNumber(),
			inPacket.GetMobileNumber().c_str(), result,
			record.strAuthCode.c_str());

	outQueue.Push(response);
	return true;
}

Protocol::RESULT Protocol::onAuthorize(Packet& inPacket)
{
	DataStore data;
	return GeneralResponsePacket::SUCCESS;
}

bool Protocol::dispatch(Packet& inPacket)
{
	bool bResponsed = false;

	switch (inPacket.GetMessageID())
	{
	case Packet::TERMINAL_REGIST:
		bResponsed = onRegist(inPacket);
		break;
	case Packet::MESSAGE_UNKNOWN:
		m_bCloseConnect = true;
		break;
	case Packet::TERMINAL_AUTHORIZE:
		bAuthorized = true;
		bResponsed = generalRespons(inPacket, onAuthorize(inPacket));
		break;
	default:
		bResponsed = generalRespons(inPacket,
				GeneralResponsePacket::NO_SUPPORTED);
		break;
	}
	return bResponsed;
}
