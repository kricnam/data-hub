/*
 * AccessServer.h
 *
 *  Created on: Sep 28, 2012
 *      Author: mxx
 */

#ifndef ACCESSSERVER_H_
#define ACCESSSERVER_H_

#include <map>
#include <string>
#include "cppsocket/tcpconnection.h"
#include "cppsocket/tcpserver.h"
#include "DeviceTable.h"
#include "ProcessAgent.h"
#include "ProbeDevice.h"
#include <ev++.h>
#include "DeviceTable.h"

using namespace std;
using namespace CPPSocket;
using namespace ev;

#define MAX_EVENTS 10000

class AccessServer:public virtual  ProcessAgent
{
public:
	AccessServer();
	virtual ~AccessServer();
	void Start(int nPort = 0);
	void Run(void);
	void Process(unsigned int event);
	static bool bStop;
protected:
	DeviceTable clients;
	TCPServer server;
	int nPort;
    io incomeWatcher;
    void onIncomeConnect(io& watcher,int revent);

};

#endif /* ACCESSSERVER_H_ */
