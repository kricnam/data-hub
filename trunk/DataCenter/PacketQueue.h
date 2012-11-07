/*
 * PacketQueue.h
 *
 *  Created on: 2011-08-31
 *      Author: mxx
 */

#ifndef PACKETQUEUE_H_
#define PACKETQUEUE_H_
#include <list>
#include "Packet.h"

class PacketQueue
{
public:
	PacketQueue();
	virtual ~PacketQueue();
	void Push(Packet& data);
	void Pop(void);
	int GetSize(void)
	{
		return queue.size();
	};
	Packet& Front(void);
	Packet& GetAt(int i);
	void Save(const char* szFile);
	void Load(const char* szFile);
	void Clear(void) { queue.clear(); }

protected:
	list<Packet> queue;
	int npopcount;
};


#endif /* PACKETQUEUE_H_ */
