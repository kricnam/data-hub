//============================================================================
// Name        : DataCenter.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include "AccessServer.h"
int main(void) {
	puts("Hello World!!!");
	AccessServer server;
	server.Start(6666);
	return EXIT_SUCCESS;
}
