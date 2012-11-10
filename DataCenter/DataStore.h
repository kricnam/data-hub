/*
 * DataStore.h
 *
 *  Created on: Nov 10, 2012
 *      Author: mxx
 */

#ifndef DATASTORE_H_
#define DATASTORE_H_
#include <string>

using namespace std;

class DataStore
{
public:
	DataStore();
	virtual ~DataStore();

	typedef struct _Vehicle_Record
	{
		int nProvince;
		int nCity;
		string Manufacture;
		string TerminalType;
		string TerminalID;
		char cTermialColor;
		string VechicleID;
		string TerminalMobileNumber;
		string strAuthCode;
	} Vehicle_Record;

	bool RegisteTerminal(Vehicle_Record& record);
	bool FindVechileRecord(const char* szVechicleID);
	bool FindTerminalRecord(const char* szTerminalID);
	bool GetVechicleRecord(Vehicle_Record& record);
};

#endif /* DATASTORE_H_ */
