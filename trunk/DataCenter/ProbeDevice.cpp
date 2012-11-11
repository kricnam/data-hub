/*
 * ProbeDevice.cpp
 *
 *  Created on: Oct 9, 2012
 *      Author: mxx
 */

#include "ProbeDevice.h"
#include "cppsocket/tcpconnection.h"
#include <ev++.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "TraceLog.h"
#include "AccessServer.h"

using namespace ev;
using namespace CPPSocket;
using namespace std;

ProbeDeviceAgent::ProbeDeviceAgent()
{
	nPort = 0;
	ptrSrv = NULL;
}

ProbeDeviceAgent::~ProbeDeviceAgent()
{
	IOReadWatch.stop();
	DisconnectSignal.stop();
	TimerWatch.stop();

	TRACE("agent vanished");
}

void ProbeDeviceAgent::OnReadClient(io& watcher, int revent)
{
	TRACE("%s:%u,[%d]", strIP.c_str(), nPort, revent);


	do
	{
		char buffer[4096];
		int n = clientSocket.recv(buffer, 4096, 0);
		if (n > 0)
		{
			TRACE("read %d bytes", n);
			Packet packet;
			strCache.append(buffer, n);
			if (packet.Parse(strCache))
			{
				if (protocol.Response(packet))
				{
					if (!IOWriteWatch.is_active())
						IOWriteWatch.start();
				}
				else
				{
					if (protocol.m_bCloseConnect)
						break;
				}
			}
			return;
		}
	} while (false);
	DisconnectSignal.send();
}

void ProbeDeviceAgent::OnTimeOut(timer& watcher, int revent)
{
	TRACE("timer");
	TimerWatch.stop();
	IOWriteWatch.start();
}

void ProbeDeviceAgent::OnDisconnect(async& watcher, int revent)
{
	IOReadWatch.stop();
	DisconnectSignal.stop();
	TimerWatch.stop();
	clientSocket.close();
	if (ptrSrv)
		ptrSrv->OnDisconnect(*this);
	TRACE("client removed");
}

void ProbeDeviceAgent::OnWriteClient(io& watcher, int revent)
{
	TRACE("enter");
	if (protocol.SendQueueReady())
	{
		string& strData = protocol.GetSendData();
		try
		{
			clientSocket.send(strData.data(), strData.size(), MSG_DONTWAIT);
		} catch (LibCException& e)
		{
			if (e.getCode() != EAGAIN)
			{
				DisconnectSignal.send();
			}
		}
	}
	else
	{
		IOWriteWatch.stop();
		time_t timeout = protocol.GetResponseTimeOut();
		TRACE("set timeout %u",timeout);
		if (timeout)
		{
			TimerWatch.repeat = timeout;
			TimerWatch.again();
		}
	}
}

void ProbeDeviceAgent::SetConnect(int socket)
{
	clientSocket.attach(socket);

	IOReadWatch.set<ProbeDeviceAgent, &ProbeDeviceAgent::OnReadClient>(this);
	IOReadWatch.set(socket, ev::READ);
	IOReadWatch.start();

	IOWriteWatch.set<ProbeDeviceAgent, &ProbeDeviceAgent::OnWriteClient>(this);
	IOWriteWatch.set(socket, ev::WRITE);
	IOWriteWatch.start();

	DisconnectSignal.set<ProbeDeviceAgent, &ProbeDeviceAgent::OnDisconnect>(
			this);
	DisconnectSignal.start();

	TimerWatch.set<ProbeDeviceAgent, &ProbeDeviceAgent::OnTimeOut>(this);
	TimerWatch.repeat = 5;
	TimerWatch.start();

}

