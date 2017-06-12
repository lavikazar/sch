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
* 4) _weight: The weight of the packet.								*
* 5) _sourceAddr: The IP address from which the packet sent.		*
* 6) _sourcePort: The port from which the packet sent.				*
* 7) _destAddr: The IP address to which the packet is heading.		*
* 8) _destPort: The port to which the packet is heading.			*
*																	*
* Description:														*
* The packet suppose to serve the Flow class which holds the		*
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
	Packet();							//Empty constructor.
	Packet(long int ID, long int time, string sourceAddr,
		string sourcePort, string destAddr, string destPort,
		int length, int weight);		//Constructor that initialize all the members.

	//Getters
	int packet_GetID();					//Gets the ID of the packet.
	int packet_GetTime();				//Gets the time that the packet arrived.
	int packet_GetLength();				//Gets the length of the packet (in Bytes).
	int packet_GetWeight();				//Gets the weight of the packet.
	string packet_GetSourceAddr();		//Gets the source address that the packet came from.
	string packet_GetSourcePort();		//Gets the source port that the packet came from.
	string packet_GetDestAddr();		//Gets the destination address that the packet is heading.
	string packet_GetDestPort();		//Gets the destination port that the packet is heading.

	//Setters
	void packet_SetWeight(int weight);	//Sets the weight of the packet.
};
#endif
