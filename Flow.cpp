/***********************************
*            Includes              *
***********************************/
#include "Flow.h"


/***********************************
*             Functions	           *
***********************************/

//Constructor
Flow::Flow()
{
}

Flow::Flow(int weight, int credit)
{
	_weight = weight;
	_credit = credit;
}

//Getters
int Flow::flow_GetWeight()
{
	return _weight;
}
int Flow::flow_GetCredit()
{
	return _credit;
}
Queue Flow::flow_GetQueue()
{
	return _Packets;
}

//Setters
void Flow::flow_IncCredit(int credit)
{
	_credit += credit;
}

void Flow::flow_SetCredit(int credit)
{
	_credit = credit;
}

//Generals
void Flow::flow_InsertToQueue(Packet pkt)
{
	_Packets.push(pkt);
}

void Flow::flow_PopFromQueue()
{
	_Packets.pop();
}