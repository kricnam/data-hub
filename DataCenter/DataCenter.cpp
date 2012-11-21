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
#include <unistd.h>
int main(void)
{
	SETTRACELEVEL(0);
	AccessServer::SetSignalHander();
	int nErrCount = 0;
	start: try
	{
		AccessServer svr;
		svr.Start(9999);
		svr.Run();
	} catch (Exception& e)
	{
		ERROR(e.what());
		if (nErrCount++ < 6)
		{
			INFO("will try in 10 seconds...");
			sleep(10);
			goto start;
		}
		else
			return 1;
	}
	return 0;
}
