/*
 * DeviceTable.cpp
 *
 *  Created on: Oct 9, 2012
 *      Author: mxx
 */

#include "DeviceTable.h"
#include "ProbeDevice.h"
#include <list>
using namespace std;

DeviceTable::DeviceTable()
{
	// TODO Auto-generated constructor stub

}

DeviceTable::~DeviceTable()
{
	// TODO Auto-generated destructor stub
}

ProbeDeviceAgent& DeviceTable::AllocAgent(void)
{
	if (freeDevice.empty())
	{
		//allocate instance
		if ((devicePool.size()+1) >= devicePool.capacity())
		{
			devicePool.reserve(10 + devicePool.capacity());
		}

		ProbeDeviceAgent* agent = new ProbeDeviceAgent;
		refDevice ref;
		ref.ptrDevice = agent;
		devicePool.push_back(ref);
		freeDevice.push_back(ref);
	}
	ProbeDeviceAgent& agent = *(freeDevice.front().ptrDevice);
	freeDevice.pop_front();
	return  agent;
}

ProbeDeviceAgent& DeviceTable::SetWorkingOn(const char* szID,
		ProbeDeviceAgent& device)
{
	refDevice ref;
	ref.ptrDevice =  &device;
	workingDevice[szID] = ref;
	return device;
}

ProbeDeviceAgent& DeviceTable::ReleaseDevice(const char* szID)
{
	map<string,refDevice>::iterator it;

	refDevice ref;

	it = workingDevice.find(szID);

	if (it == workingDevice.end()) throw exception();

	ref.ptrDevice = workingDevice[szID].ptrDevice;
	ProbeDeviceAgent& agent = *ref.ptrDevice;
	freeDevice.push_back(ref);
	workingDevice.erase(it);
	return agent;
}


