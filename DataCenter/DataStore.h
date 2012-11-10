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
	} Vehicle_Record;

	bool UpdateTerminalRecord(Vehicle_Record& record);
	bool FindTerminalRecord(const char* VechicleID);
};

#endif /* DATASTORE_H_ */
