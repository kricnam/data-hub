/*
 * Packet_test.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: mxx
 */

#include "Packet.h"
#include "TraceLog.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
int main(int argc,char** argv)
{
	SETTRACELEVEL(0);
	string strBody = "ABCDEFG";
	char c = 0x7e;
	strBody.append(1,c);
	strBody.append(1,0x7d);
	DUMP(strBody.data(),strBody.size());
	Packet packet(Packet::TERMINAL_GENERAL_HEARTBEAT,"123456",strBody);
	string strPacket = packet.PackMessage();
	DUMP(strPacket.data(),strPacket.size());
	if (packet.Parse(strPacket))
	{
		INFO("Parse OK");
		DUMP(packet.GetContent().data(),packet.GetContent().size());
		INFO("%s",packet.GetContent().c_str());

	}
	INFO("test over");

}
