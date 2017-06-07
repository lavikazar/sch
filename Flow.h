#ifndef Flow_H
#define Flow_H

/***********************************
*            Includes              *
***********************************/
#include "Generals.h"
#include "Packet.h"
using namespace std;

/***********************************
*        Structs & Defines         *
***********************************/
typedef std::queue<Packet> Queue;

/********************************************************************
* Class Flow declaration:											*
*																	*
* The class defines a flow with the following members:				*
* 1) _weight : The weight of flow (weight of each packet)			*
* 2) _credit: The credit left for this flow							*
* 3) _creditUsed: The credit that was used already for this packet	*
* 4) _Packets: The queue that holds all the packets of the flow		*
*																	*
* Description:														*
* The FlowQueue suppose to serve the hash map of the scheduler.		*
********************************************************************/

class Flow
{
	/* Private Members */
private:
	int		_weight;
	int		_credit;
	int		_creditUsed;
	int		_priority;
	Queue	_Packets;

	/* Public Class functions */
public:

	//Constructor
	Flow();
	Flow(int weight, int credit, int creditUsed);

	//Getters
	int flow_GetWeight();
	int flow_GetCredit();
	int flow_GetCreditUsed();
	int Flow::flow_GetPriority();
	Queue flow_GetQueue();
	//Given a packet, insert it to the flow
	void flow_InsertToQueue(Packet pkt);

	//Setters
	void Flow::flow_IncCredit(int credit);
	void Flow::flow_SetCredit(int credit);
};

#endif