/*
 * DeviceTable.h
 *
 *  Created on: Oct 9, 2012
 *      Author: mxx
 */

#ifndef DEVICETABLE_H_
#define DEVICETABLE_H_
#include "DeviceAgent.h"
#include <string>
#include <list>
#include <set>
#include <vector>

using namespace std;
class DeviceAgent;

class DeviceTable
{
public:
	DeviceTable();
	virtual ~DeviceTable();
	DeviceAgent& AllocAgent(void);
	DeviceAgent& SetWorkingOn(const char* szID,DeviceAgent& device);
	DeviceAgent& ReleaseDevice(DeviceAgent& devic);
	void ReleaseAll(void);

protected:
	typedef DeviceAgent* refDevice;

	vector<refDevice>  devicePool;
	list<refDevice> freeDevice;
	set<refDevice> workingDevice;
};

#endif /* DEVICETABLE_H_ */
