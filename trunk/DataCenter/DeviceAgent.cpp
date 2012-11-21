/*
 * ProbeDevice.cpp
 *
 *  Created on: Oct 9, 2012
 *      Author: mxx
 */

#include "DeviceAgent.h"
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

DeviceAgent::DeviceAgent()
{
	nPort = 0;
	ptrSrv = NULL;
}

DeviceAgent::~DeviceAgent()
{
	IOReadWatch.stop();
	DisconnectSignal.stop();
	TimerWatch.stop();

	TRACE("agent vanished");
}

void DeviceAgent::OnReadClient(io& watcher, int revent)
{
	TRACE("%s:%u,[%d]", strIP.c_str(), nPort, revent);

	char buffer[4096];
	int n = 0;
	try
	{
		n = clientSocket.recv(buffer, 4096, 0);
	} catch (Exception& e)
	{
		DEBUG(e.what());
	}

	if (n > 0)
	{
		TRACE("read %d bytes", n);
		DUMP(buffer, n);
		Packet packet;
		strCache.append(buffer, n);
		while (packet.Parse(strCache))
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
		};
		return;
	}

	DisconnectSignal.send();
}

void DeviceAgent::OnTimeOut(timer& watcher, int revent)
{
	TRACE("timer");
	TimerWatch.stop();
	IOWriteWatch.start();
}

void DeviceAgent::OnDisconnect(async& watcher, int revent)
{
	IOReadWatch.stop();
	DisconnectSignal.stop();
	TimerWatch.stop();
	clientSocket.close();
	if (ptrSrv)
		ptrSrv->OnDisconnect(*this);
	TRACE("client removed");
}

void DeviceAgent::OnWriteClient(io& watcher, int revent)
{
	TRACE("enter");
	if (protocol.SendQueueReady())
	{
		string& strData = protocol.GetSendData();
		try
		{
			clientSocket.send(strData.data(), strData.size(), MSG_DONTWAIT);
			protocol.OnSendResponseOK();
		} catch (LibCException& e)
		{
			if (e.getCode() != EAGAIN && e.getCode() != EWOULDBLOCK)
			{
				DisconnectSignal.send();
			}
		}
	}
	else
	{
		IOWriteWatch.stop();
		time_t timeout = protocol.GetResponseTimeOut();
		TRACE("set timeout %u", timeout);
		if (timeout)
		{
			TimerWatch.repeat = timeout;
			TimerWatch.again();
		}
	}
}

void DeviceAgent::SetConnect(int socket)
{
	clientSocket.attach(socket);

	IOReadWatch.set<DeviceAgent, &DeviceAgent::OnReadClient>(this);
	IOReadWatch.set(socket, ev::READ);
	IOReadWatch.start();

	IOWriteWatch.set<DeviceAgent, &DeviceAgent::OnWriteClient>(this);
	IOWriteWatch.set(socket, ev::WRITE);
	IOWriteWatch.start();

	DisconnectSignal.set<DeviceAgent, &DeviceAgent::OnDisconnect>(this);
	DisconnectSignal.start();

	TimerWatch.set<DeviceAgent, &DeviceAgent::OnTimeOut>(this);
	TimerWatch.repeat = 5;
	TimerWatch.start();

}

