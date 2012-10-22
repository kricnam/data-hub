/*
 * AccessServer.h
 *
 *  Created on: Oct 22, 2012
 *      Author: mxx
 */

#ifndef ACCESSSERVER_H_
#define ACCESSSERVER_H_

#include "cppsocket/tcpserver.h"

using namespace CPPSocket;

class AccessServer
{
public:
	AccessServer();
	virtual ~AccessServer();
	//start server on listen port
	void Start(int nPort);
protected:
	TCPServer tcp_server;
	void registerListener(void);
	void onIncomeConnect(void);
};

#endif /* ACCESSSERVER_H_ */
