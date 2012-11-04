/*
 * ProcessAgent.h
 *
 *  Created on: Oct 12, 2012
 *      Author: mxx
 */

#ifndef PROCESSAGENT_H_
#define PROCESSAGENT_H_

class ProcessAgent
{
public:
	ProcessAgent() {};
	virtual ~ProcessAgent(){};
	virtual void Process(unsigned int event) = 0;
};


#endif /* PROCESSAGENT_H_ */
