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
#include <list>
#include <set>
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
	ProbeDeviceAgent& ReleaseDevice(ProbeDeviceAgent& devic);
	void ReleaseAll(void);

protected:
	typedef ProbeDeviceAgent* refDevice;

	vector<refDevice>  devicePool;
	list<refDevice> freeDevice;
	set<refDevice> workingDevice;
};

#endif /* DEVICETABLE_H_ */
