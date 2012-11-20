/*
 * DeviceTable.cpp
 *
 *  Created on: Oct 9, 2012
 *      Author: mxx
 */

#include "DeviceTable.h"
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

DeviceAgent& DeviceTable::AllocAgent(void)
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
		DeviceAgent* agent = new DeviceAgent;
		refDevice ref;
		ref = agent;
		devicePool.push_back(ref);
		freeDevice.push_back(ref);
	}
	DeviceAgent& agent = *(freeDevice.front());
	freeDevice.pop_front();
	workingDevice.insert(&agent);
	DEBUG("pool %d,free %d,used %d",devicePool.size(),freeDevice.size(),workingDevice.size());
	return  agent;
}


DeviceAgent& DeviceTable::ReleaseDevice(DeviceAgent& device)
{
	set<refDevice>::iterator it = workingDevice.find(&device);

	if (it == workingDevice.end()) throw exception();
	workingDevice.erase(it);
	freeDevice.push_back(&device);
	DEBUG("pool %d,free %d,used %d",devicePool.size(),freeDevice.size(),workingDevice.size());
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
