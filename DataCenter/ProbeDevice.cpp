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
	TRACE("%s:%u,[%d]",strIP.c_str(),nPort,revent);
	do
	{
		char buffer[4096];
		int n = clientSocket.recv(buffer,4096,0);
		if (n > 0 )
		{
			TRACE("read %d bytes", n);
			Packet packet;
			strCache.append(buffer,n);
			reponseClient(protocol.Response(packet.UnPackMessage(strCache)));
		}
		else
		{
			DisconnectSignal.send();
		}
	} while (false);
}

void ProbeDeviceAgent::OnTimeOut(timer& watcher, int revent)
{
	TRACE("timer");
	//TimerWatch.again();
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
	IOWriteWatch.stop();
}

void ProbeDeviceAgent::SetConnect(int socket)
{
	clientSocket.attach(socket);

	IOReadWatch.set<ProbeDeviceAgent,&ProbeDeviceAgent::OnReadClient>(this);
	IOReadWatch.set(socket,ev::READ);
	IOReadWatch.start();

	IOWriteWatch.set<ProbeDeviceAgent,&ProbeDeviceAgent::OnWriteClient>(this);
	IOWriteWatch.set(socket,ev::WRITE);
	IOWriteWatch.start();

	DisconnectSignal.set<ProbeDeviceAgent,&ProbeDeviceAgent::OnDisconnect>(this);
	DisconnectSignal.start();

	TimerWatch.set<ProbeDeviceAgent,&ProbeDeviceAgent::OnTimeOut>(this);
	TimerWatch.repeat = 30;
	TimerWatch.start();



}

void ProbeDeviceAgent::reponseClient(Packet& outPacket)
{
	try
	{
		if (&outPacket)
			clientSocket.send(outPacket.GetData(),outPacket.GetSize(),MSG_DONTWAIT);
	}
	catch(LibCException& e)
	{
		if (e.getCode() == EAGAIN)
		{
			//push back to queue,and send later
		}
	}

}
