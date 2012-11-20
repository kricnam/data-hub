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
#include "DeviceAgent.h"
#include <ev++.h>
#include "DeviceTable.h"
#include <set>


using namespace std;
using namespace CPPSocket;
using namespace ev;

#define MAX_EVENTS 10000
class DeviceTable;
class DeviceAgent;
class AccessServer:public virtual  ProcessAgent
{
public:
	AccessServer();
	virtual ~AccessServer();
	void Start(int nPort = 0);
	void Run(void);
	void Stop();
	void Process(unsigned int event);
	void OnDisconnect(DeviceAgent& agent);
	static void SetSignalHander(void);
	static void SignalHandle(sig& water, int revent);
	static bool bStop;

protected:
	DeviceTable clients;
	TCPServer server;
	int nPort;
    io incomeWatcher;
    void onIncomeConnect(io& watcher,int revent);

    static set<AccessServer*> servers;
    static sig signal_watcher_int;
    static sig signal_watcher_term;
    static void registerServer(AccessServer* ptr);

};

#endif /* ACCESSSERVER_H_ */
