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

using namespace ev;
using namespace CPPSocket;
using namespace std;

ProbeDeviceAgent::ProbeDeviceAgent()
{
	nPort = 0;
	nSocket = -1;
	ptrTable = NULL;
}

ProbeDeviceAgent::~ProbeDeviceAgent()
{

}

void ProbeDeviceAgent::OnReadClient(io& watcher, int revent)
{
	TRACE("%s:%u,[%d]",strIP.c_str(),nPort,revent);
	string str;
	do
	{
		char buffer[4096];
		int n = recv(nSocket,buffer,4096,0);
		if (n > 0 )
		{
			TRACE("read %d bytes", n);

		}
		else
		{
			if (n == -1)
				TRACE("error read");
			else
				TRACE("read 0 byte");
			DisconnectSignal.send();
		}
	} while (false);
}

void ProbeDeviceAgent::OnTimeOut(timer& watcher, int revent)
{
	TRACE("timer");
	TimerWatch.again();
}

void ProbeDeviceAgent::OnDisconnect(async& watcher, int revent)
{
	IOReadWatch.stop();
	DisconnectSignal.stop();
	TimerWatch.stop();
	close(nSocket);
	nSocket = -1;
	if (ptrTable)
		ptrTable->ReleaseDevice(strDevID.c_str());
	TRACE("client removed");
}

void ProbeDeviceAgent::SetConnect(int socket)
{
	IOReadWatch.set<ProbeDeviceAgent,&ProbeDeviceAgent::OnReadClient>(this);
	IOReadWatch.set(socket,ev::READ);
	IOReadWatch.start();
	nSocket = socket;

	DisconnectSignal.set<ProbeDeviceAgent,&ProbeDeviceAgent::OnDisconnect>(this);
	DisconnectSignal.start();

	TimerWatch.set<ProbeDeviceAgent,&ProbeDeviceAgent::OnTimeOut>(this);
	TimerWatch.repeat = 30;
}

