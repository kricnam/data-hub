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
#include "Packet.h"
#include "PacketQueue.h"
#include "Protocol.h"
using namespace std;
using namespace ev;
using namespace CPPSocket;

class AccessServer;
class ProbeDeviceAgent
{
public:
	ProbeDeviceAgent();
	virtual ~ProbeDeviceAgent();
	virtual void Process(int revent) {};
	void OnReadClient(io& watcher,int revent);
	void OnWriteClient(io& watcher,int revent);
	void OnTimeOut(timer& watcher,int revent);
	void OnDisconnect(async& watcher,int revent);
	void SetConnect(int socket);
	void SetClientAddress(const char* szIP, int nPort)
	{
		strIP = szIP;
		this->nPort = nPort;
	};
	void SetServer(AccessServer* ptr) { ptrSrv = ptr;};
	string strDevID; // A unique Device ID

protected:
	Protocol protocol;
	AccessServer* ptrSrv;
	Socket clientSocket;
	io IOReadWatch;
	io IOWriteWatch;
	timer TimerWatch;
	async DisconnectSignal;
	string strIP;
	string strCache;
	int nPort;

	void reponseClient(Packet& outPacket);
};

#endif /* PROBEDEVICE_H_ */
