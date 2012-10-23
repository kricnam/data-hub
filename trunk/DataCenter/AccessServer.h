/*
 * AccessServer.h
 *
 *  Created on: Oct 22, 2012
 *      Author: mxx
 */

#ifndef ACCESSSERVER_H_
#define ACCESSSERVER_H_

#include "cppsocket/tcpserver.h"
#include <ev++.h>

using namespace CPPSocket;
using namespace ev;

class AccessServer
{
public:
	AccessServer();
	virtual ~AccessServer();
	//start server on listen port
	void Start(int nPort);
protected:
	TCPServer tcp_server;
	io incomeWatcher;
	void onIncomeConnect(io& watcher,int revent);
};

#endif /* ACCESSSERVER_H_ */
