/*
 * AccessServer.cpp
 *
 *  Created on: Sep 28, 2012
 *      Author: mxx
 */

#include "AccessServer.h"
#include "TraceLog.h"
#include <sys/epoll.h>
#include <exception>
#include <ev++.h>
#include <stdio.h>
#include <stdlib.h>
#include "cppsocket/socket.h"
#include "ProbeDevice.h"
#include "TraceLog.h"

using namespace std;
using namespace ev;

bool AccessServer::bStop = false;

AccessServer::AccessServer()
{
	nPort = 6666;
}

AccessServer::~AccessServer()
{

}

void AccessServer::Start(int nPort)
{
	server.start(nPort,10);
	incomeWatcher.set<AccessServer,&AccessServer::onIncomeConnect>(this);
	incomeWatcher.set(server.getSocket(),ev::READ);
	incomeWatcher.start();
}

void AccessServer::Run(void)
{
	get_default_loop().run();
}

void AccessServer::Process(unsigned int event)
{
}

void AccessServer::onIncomeConnect(io& watcher, int revent)
{
	Address address;
	int clientSocket;
	clientSocket = server.acceptSocket(address);
	ProbeDeviceAgent& agent=clients.AllocAgent();
	agent.SetConnect(clientSocket);
	agent.SetClientAddress(address.getIPString().c_str(),address.getPort());
	INFO("client %s,%u",address.getIPString().c_str(),address.getPort());
}







