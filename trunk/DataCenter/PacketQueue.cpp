/*
 * PacketQueue.cpp
 *
 *  Created on: 2011-08-31
 *      Author: mxx
 */

#include "PacketQueue.h"
#include "TraceLog.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

PacketQueue::PacketQueue()
{
	npopcount = 0;
}

PacketQueue::~PacketQueue()
{

}

void PacketQueue::Push(Packet& data)
{

	TRACE("data,size %d",data.GetSize());
	if (data.GetSize())
	{
		try
		{
			queue.push_back(data);
		}
		catch(exception& ex)
		{
			ERROR(ex.what());
		}
	}
}

Packet& PacketQueue::Front(void)
{
	return queue.front();
}

Packet& PacketQueue::GetAt(int i)
{
	list<Packet>::iterator it;
	int count = 0;
	for(it = queue.begin();it!=queue.end();it++)
	{
		if (count++== i) break;
	}
	return (*it);
}

void PacketQueue::Pop(void)
{
	try
	{
		queue.pop_front();
	}
	catch(exception &ex)
	{
		ERROR(ex.what());
	}
}

void PacketQueue::Save(const char* szFile)
{
	DEBUG("Save to %s",szFile);
	int fd = open(szFile,O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
	if (fd < 0)
	{
		ERRTRACE();
		return;
	}
	list<Packet>::iterator it;
	for(it = queue.begin();it!=queue.end();it++)
	{
		int n = (*it).GetSize();
		if (n)
		{
			if (write(fd,&n,sizeof(n))!=sizeof(n))
			{
				ERRTRACE();
				break;
			}
			if (n!=write(fd,(*it).GetData(),n))
			{
				ERRTRACE();
				break;
			}
		}
	}
//	if (fdatasync(fd)<0)
//		ERRTRACE();
	if (close(fd)<0)
		ERRTRACE();
}

void PacketQueue::Load(const char* szFile)
{
	INFO("Load from %s",szFile);
	FILE* fd = fopen(szFile,"rb");
	if (fd == NULL)
	{
		ERRTRACE();
		return;
	}
	queue.clear();
	while(!feof(fd))
	{
		int n;
		if (fread(&n,sizeof(n),1,fd)< 1)
			break;

		if (n>0 && n < 5000)
		{
			char* buf = new char[n];
			if (buf && (fread(buf,n,1,fd)== 1))
			{
				Packet packet;

				if (packet.GetSize()) Push(packet);
				else
				{
					WARNING("Invalid data");
					DUMP(packet.GetData(),packet.GetSize());
				}
			}
			else
			{
				WARNING("Invalid data,file maybe corrupted.");
			}
			if (buf) delete buf;
		}
		else
		{
			WARNING("Corrupted data, move to next");
		}
	}
	fclose(fd);
	//if (remove(szFile)!=0)
	//	ERRTRACE();
	INFO("Total %d records loaded",queue.size());
}


