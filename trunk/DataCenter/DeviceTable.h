/*
 * DeviceTable.h
 *
 *  Created on: Oct 9, 2012
 *      Author: mxx
 */

#ifndef DEVICETABLE_H_
#define DEVICETABLE_H_
#include "ProbeDevice.h"
#include <string>
#include <map>
#include <list>
#include <vector>


using namespace std;
class ProbeDeviceAgent;
class DeviceTable
{
public:
	DeviceTable();
	virtual ~DeviceTable();
	ProbeDeviceAgent& AllocAgent(void);
	ProbeDeviceAgent& SetWorkingOn(const char* szID,ProbeDeviceAgent& device);
	ProbeDeviceAgent& ReleaseDevice(const char* szID);

protected:
	typedef struct _refDevice
	{
		ProbeDeviceAgent* ptrDevice;
	} refDevice;

	vector<refDevice>  devicePool;
	list<refDevice> freeDevice;
	map<string,refDevice> workingDevice;
};

#endif /* DEVICETABLE_H_ */
