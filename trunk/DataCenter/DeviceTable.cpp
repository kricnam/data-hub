/*
 * DeviceTable.cpp
 *
 *  Created on: Oct 9, 2012
 *      Author: mxx
 */

#include "DeviceTable.h"
#include "ProbeDevice.h"
#include "TraceLog.h"
using namespace std;

DeviceTable::DeviceTable()
{
	// TODO Auto-generated constructor stub

}

DeviceTable::~DeviceTable()
{
	// TODO delete all allocated memory
}

ProbeDeviceAgent& DeviceTable::AllocAgent(void)
{
	if (freeDevice.empty())
	{
		//allocate instance
		if ((devicePool.size()+1) >= devicePool.capacity())
		{
			TRACE("realloc space");
			devicePool.reserve(10 + devicePool.capacity());
		}
		DEBUG("alloc new memory");
		ProbeDeviceAgent* agent = new ProbeDeviceAgent;
		refDevice ref;
		ref = agent;
		devicePool.push_back(ref);
		freeDevice.push_back(ref);
	}
	ProbeDeviceAgent& agent = *(freeDevice.front());
	freeDevice.pop_front();
	workingDevice.insert(&agent);
	DEBUG("pool %d,free %d,used %d",devicePool.size(),freeDevice.size(),workingDevice.size());
	return  agent;
}


ProbeDeviceAgent& DeviceTable::ReleaseDevice(ProbeDeviceAgent& device)
{
	set<refDevice>::iterator it = workingDevice.find(&device);

	if (it == workingDevice.end()) throw exception();
	workingDevice.erase(it);
	freeDevice.push_back(&device);
	DEBUG("pooi %d,free %d,used %d",devicePool.size(),freeDevice.size(),workingDevice.size());
	return device;
}

void DeviceTable::ReleaseAll(void)
{
	vector<refDevice>::iterator it;
	for(it = devicePool.begin();it != devicePool.end() ; it++)
	{
		delete (*it);
	}
}
