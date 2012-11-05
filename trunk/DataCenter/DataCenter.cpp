//============================================================================
// Name        : DataCenter.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdlib.h>
#include "AccessServer.h"
#include "TraceLog.h"
#include "DeviceTable.h"
#include "ProbeDevice.h"

int main(void)
{
	SETTRACELEVEL(0);
	AccessServer::SetSignalHander();
	AccessServer svr;
	svr.Start(6666);
	svr.Run();
}
