/*
 * DataStore.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: mxx
 */

#include "DataStore.h"

DataStore::DataStore()
{
	// TODO Auto-generated constructor stub

}

DataStore::~DataStore()
{
	// TODO Auto-generated destructor stub
}

bool DataStore::RegisteTerminal(Vehicle_Record& record)
{
	return true;
}

bool DataStore::FindVechileRecord(const char* szVechicleID)
{
	return true;
}

bool DataStore::FindTerminalRecord(const char* TerminalID)
{
	return true;
}

bool DataStore::GetVechicleRecord(Vehicle_Record& record)
{
	record.strAuthCode = "1234567890";
	return true;
}
