/*
 * RegistPacket.h
 *
 *  Created on: Nov 10, 2012
 *      Author: mxx
 */

#ifndef REGISTPACKET_H_
#define REGISTPACKET_H_

#include "Packet.h"

class RegistPacket: public virtual Packet
{
public:
	RegistPacket();
	RegistPacket(Packet& packet);
	virtual ~RegistPacket();

	typedef struct _TerminalRegiste
	{
		WORD Province;
		WORD City;
		BYTE Manufacture[5];
		BYTE TerminalType[8];
		BYTE TerminalID[7];
		BYTE BodyColor;
		BYTE VIN[1];
	} __attribute__ ((packed)) TerminalRegiste;

	string& GetTerminalVIN();
	int GetProvinceCode();
	int GetCityCode();
	string& GetManufacture();
	string& GetType();
	string& GetTerminalID();
	int GetColor();

protected:
	string strVIN;
	int nProvince;
	int nCity;
	string strManufacture;
	string strType;
	string strID;
	int nColorCode;
};

#endif /* REGISTPACKET_H_ */
