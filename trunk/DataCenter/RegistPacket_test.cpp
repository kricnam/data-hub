/*
 * RegistPacket_test.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: mxx
 */

#include "RegistPacket.h"
#include "Packet.h"
#include "TraceLog.h"
#ifdef _PACKET_TEST_
int main(int argc,const char** argv)
{
	SETTRACELEVEL(0);
	Packet p;
	p.SetMessageID(Packet::TERMINAL_REGIST);
	RegistPacket r(p);
	r.Dump();
}




#endif
