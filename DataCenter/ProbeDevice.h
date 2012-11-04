/*
 * ProbeDevice.h
 *
 *  Created on: Oct 9, 2012
 *      Author: mxx
 */

#ifndef PROBEDEVICE_H_
#define PROBEDEVICE_H_
#include <string>
#include "ProcessAgent.h"
#include <ev++.h>
#include "cppsocket/tcpconnection.h"
#include "DeviceTable.h"

using namespace std;
using namespace ev;
using namespace CPPSocket;
class DeviceTable;
class ProbeDeviceAgent
{
public:
	ProbeDeviceAgent();
	virtual ~ProbeDeviceAgent();
	virtual void Process(int revent) {};
	void OnReadClient(io& watcher,int revent);
	void OnTimeOut(timer& watcher,int revent);
	void OnDisconnect(async& watcher,int revent);
	void SetConnect(int socket);
	void SetClientAddress(const char* szIP, int nPort)
	{
		strIP = szIP;
		this->nPort = nPort;
	};
	string strDevID; // A unique Device ID
	DeviceTable* ptrTable;
protected:
	int nSocket;
	io IOReadWatch;
	timer TimerWatch;
	async DisconnectSignal;
	string strIP;
	int nPort;
};

#endif /* PROBEDEVICE_H_ */
