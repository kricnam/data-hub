/*
 * AccessServer.cpp
 *
 *  Created on: Oct 22, 2012
 *      Author: mxx
 */

#include "AccessServer.h"
#include <ev++.h>
AccessServer::AccessServer()
{
	// TODO Auto-generated constructor stub

}

AccessServer::~AccessServer()
{
	// TODO Auto-generated destructor stub
}

void AccessServer::Start(int nPort)
{
	tcp_server.start(nPort,10);
}
