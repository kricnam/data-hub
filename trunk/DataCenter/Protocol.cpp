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
#include "TraceLog.h"

using namespace std;

Protocol::Protocol()
{
	tResponseTimeOut = 10;
	bAuthorized = false;
	m_bCloseConnect = false;
	nRetryLimit = 3;
	tConnectTime = 0;
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
	//DataStore data;
	//TODO:compare with database
	return GeneralResponsePacket::SUCCESS;
}

string& Protocol::GetSendData(void)
{
	Packet& packet = outQueue.Front();
	packet.SetSendTime(time(NULL));
	time_t timeOut = packet.GetTimeOut();
	if (timeOut == 0)
		timeOut = tResponseTimeOut;
	timeOut = (packet.GetTransmitCount() + 1) * timeOut;
	packet.SetTimeOut(timeOut);
	return packet.PackMessage();
}

void Protocol::OnSendResponseOK(void)
{
	if (outQueue.GetSize() && outQueue.Front().m_bResponse)
		outQueue.Pop();
}

int Protocol::SendQueueReady(void)
{
	if (outQueue.GetSize() < 1)
		return 0;
	Packet& packet = outQueue.Front();
	time_t tSendTime = packet.GetSendTime();
	if (tSendTime == 0)
	{
		return 1;
	}
	else
	{
		if (packet.GetTransmitCount() > nRetryLimit)
		{
			WARNING(
					"send packet fail after %d retry", packet.GetTransmitCount());
			outQueue.Pop();
			return outQueue.GetSize();
		}
		time_t now = time(NULL);
		if ((now - tSendTime) >= packet.GetTimeOut())
			return 1;
	}

	return 0;
}

time_t Protocol::GetResponseTimeOut(void)
{
	if (outQueue.GetSize() < 1)
		return 0;
	return outQueue.Front().GetTimeOut();
}

int Protocol::Timer(void)
{
	if (tConnectTime==0)
	{
		tConnectTime = time(NULL);
		return 30;
	}

	if (time(NULL)-tConnectTime >= 30 && !bAuthorized)
	{
		m_bCloseConnect = true;
	}

	return GetResponseTimeOut();
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
		TRACE("unknown message");
		m_bCloseConnect = true;
		break;
	case Packet::TERMINAL_AUTHORIZE:
		//FIXME:set according to onAuthorize result;
		bAuthorized = true;
		bResponsed = generalRespons(inPacket, onAuthorize(inPacket));
		break;
	case Packet::TERMINAL_GENERAL_HEARTBEAT:
		bResponsed = generalRespons(inPacket,GeneralResponsePacket::SUCCESS);
		break;
	default:
		bResponsed = generalRespons(inPacket,
				GeneralResponsePacket::NO_SUPPORTED);
		break;
	}
	return bResponsed;
}
