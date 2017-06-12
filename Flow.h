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
* The Flow suppose to serve the hash map of the scheduler.			*
********************************************************************/

class Flow
{
	/* Private Members */
private:
	int		_weight;
	int		_credit;
	Queue	_Packets;

	/* Public Class functions */
public:

	//Constructor
	Flow();									//Empty constructor
	Flow(int weight, int credit);			//Constructor of a new flow. Initialize the weight and the credit by a given arguments.

	//Getters
	int flow_GetWeight();					//Gets the weight of the flow.
	int flow_GetCredit();					//Gets the credit that the flow has.
	Queue flow_GetQueue();					//Gets the queue of the flow.

	//Setters
	void Flow::flow_IncCredit(int credit);	//Increments the credit of the flow by a given 'credit'.
	void Flow::flow_SetCredit(int credit);	//Sets the credit of the flow to be 'credit'.

	//Generals
	void flow_InsertToQueue(Packet pkt);	//Inserts to the queue of the flow the given packet.
	void flow_PopFromQueue();				//Pops from the queue a single packet.
};

#endif