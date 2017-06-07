/***********************************
*            Includes              *
***********************************/
#include "Packet.h"


/***********************************
*             Functions	           *
***********************************/

//Constructor
Packet::Packet()
{
}

Packet::Packet(long int ID, long int time, string sourceAddr, string sourcePort, string destAddr, string destPort, int length, int weight)
{
	_id = ID;
	_time = time;
	_sourceAddr = sourceAddr;
	_sourcePort = sourcePort;
	_destAddr = destAddr;
	_destPort = destPort;
	_length = length;
	_weight = weight;
}

//Getters
int Packet::packet_GetID()
{
	return _id;
}
int Packet::packet_GetTime()
{
	return _time;
}
int Packet::packet_GetLength()
{
	return _length;
}
int Packet::packet_GetWeight()
{
	return _weight;
}
string Packet::packet_GetSourceAddr()
{
	return _sourceAddr;
}
string Packet::packet_GetSourcePort()
{
	return _sourcePort;
}
string Packet::packet_GetDestAddr()
{
	return _destAddr;
}
string Packet::packet_GetDestPort()
{
	return _destPort;
}

//Setters
void Packet::packet_SetWeight(int weight)
{
	_weight = weight;
}