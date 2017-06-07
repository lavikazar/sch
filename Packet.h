#ifndef Packet_H
#define Packet_H

/***********************************
*            Includes              *
***********************************/
#include "Generals.h"
using namespace std;

/********************************************************************
* Class Packet declaration:											*
*																	*
* The class defines a packet with the following members:			*
* 1) _id : The id of the packet.									*
* 2) _time: The time the packet arrived.							*
* 3) _length: The length of the data in the packet.					*
* 4) _sourceAddr: The IP address from which the packet sent.		*
* 5) _sourcePort: The port from which the packet sent.				*
* 6) _destAddr: The IP address to which the packet is heading.		*
* 7) _destPort: The port to which the packet is heading.			*
*																	*
* Description:														*
* The packet suppose to serve the FlowQueue class which holds the	*
* flow of each packet with the same: source IP, source port,		*
* destination IP, destination port.									*
********************************************************************/


class Packet
{
	/* Private Members */
private:
	long int	_id;
	long int	_time;
	int			_length;
	int			_weight;
	string		_sourceAddr;
	string		_sourcePort;
	string		_destAddr;
	string		_destPort;

	/* Public Class functions */
public:

	//Constructor
	Packet();
	Packet(long int ID, long int time, string sourceAddr, string sourcePort, string destAddr, string destPort, int length, int weight);

	//Getters
	int packet_GetID();
	int packet_GetTime();
	int packet_GetLength();
	int packet_GetWeight();
	string packet_GetSourceAddr();
	string packet_GetSourcePort();
	string packet_GetDestAddr();
	string packet_GetDestPort();

	//Setters
	void packet_SetWeight(int weight);
};

#endif
