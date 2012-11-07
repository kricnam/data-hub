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

using namespace std;
using namespace ev;

bool AccessServer::bStop = false;
set<AccessServer*> AccessServer::servers;
sig AccessServer::signal_watcher_int;
sig AccessServer::signal_watcher_term;

AccessServer::AccessServer()
{
	nPort = 6666;
}

AccessServer::~AccessServer()
{

}

void AccessServer::Start(int nPort)
{
	INFO("start listening on %d",nPort);

	server.start(nPort, 10);
	incomeWatcher.set<AccessServer, &AccessServer::onIncomeConnect>(this);
	incomeWatcher.set(server.getSocket(), ev::READ);
	incomeWatcher.start();
	registerServer(this);
}

void AccessServer::Run(void)
{
	get_default_loop().run();
}

void AccessServer::Process(unsigned int event)
{
}

void AccessServer::SetSignalHander(void)
{
	sigset_t block_set;
	signal_watcher_int.set(SIGINT);
	signal_watcher_int.set<AccessServer::SignalHandle>();
	signal_watcher_int.start();
	signal_watcher_term.set(SIGTERM);
	signal_watcher_term.set<AccessServer::SignalHandle>();
	signal_watcher_term.start();

	/* block PIPE signal */
	sigemptyset(&block_set);
	sigaddset(&block_set, SIGPIPE);
	sigprocmask(SIG_BLOCK, &block_set, NULL);
}

void AccessServer::SignalHandle(sig& water, int revent)
{
	INFO("exit");
	get_default_loop().break_loop();
	set<AccessServer*>::iterator it;
	for (it = servers.begin(); it != servers.end(); it++)
		(*it)->Stop();

}

void AccessServer::Stop()
{
	TRACE("server stop");
	clients.ReleaseAll();
	server.stop();
}

void AccessServer::OnDisconnect(ProbeDeviceAgent& agent)
{
	clients.ReleaseDevice(agent);
}

void AccessServer::onIncomeConnect(io& watcher, int revent)
{
	Address address;
	int clientSocket;
	clientSocket = server.acceptSocket(address);
	ProbeDeviceAgent& agent = clients.AllocAgent();
	agent.SetConnect(clientSocket);
	agent.SetClientAddress(address.getIPString().c_str(), address.getPort());
	agent.SetServer(this);
	INFO("client %s,%u", address.getIPString().c_str(), address.getPort());
}

void AccessServer::registerServer(AccessServer* ptr)
{
	if (servers.count(ptr) == 0)
		servers.insert(ptr);
	INFO("active server:%d", servers.size());
}
