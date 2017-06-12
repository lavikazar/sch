#ifndef Scheduler_H
#define Scheduler_H

/***********************************
*            Includes              *
***********************************/
#include "SchedulerUtils.h"
#include "Flow.h"
using namespace std;

/*************************************************************************
* Class Scheduler declaration:											 *
*																		 *
* The class defines a packet with the following members:				 *
* 1) _quantum : The quantum number for all the packets (size in bytes).	 *
* 2) _defaultWeight: The default weight of a flow with packets.			 *
* 3) _lastPacketTime: Holds the time of the last packet.				 *
* 4) _currNumOfPkts: Holds the current total number of packets.			 *
* 5) _schTime: Holds the current time of the schduler.					 *
* 3) _SchedulerType: The type of scheduler (false for RR, true for DRR). *
* 4) _inputFile: Holds the path of the input file.						 *
* 5) _outputFile: Holds the path of the output file.					 *
* 6) _lastPacket: Holds the last packet.								 *
* 6) _schedulerHashMap: Holds the keys and the flows					 *
* 7) _flowsList: Holds the list of flows.								 *
*																		 *
* Description: This class incharge of the routine of the receiving		 * 
* packets, take care of them according to the type of the scheduler		 *
* and send them.													     *
*************************************************************************/

class Scheduler
{
	/* Private Members */
private:
	int						_quantum;
	int						_defaultWeight;
	int						_lastPacketTime;
	int						_currNumOfPkts;
	long int				_schTime;
	string					_SchedulerType;
	string					_inputFile;
	string					_outputFile;
	Packet					_lastPacket;
	map<string, Flow>		_schedulerHashMap;
	vector<string>			_flowsList;

	/* Public Class functions & vars */
public:

	//Constructor
	Scheduler();									//Empty constructor
	Scheduler(int quantom, int defaultWeight,
		int lastTimePacket, string SchedulerType,
		string inputFile, string outputFile);		//Constructor that intialize all the relevant members

	//Getters
	int GetQuantum();								//Gets the quantom of the scheduler
	int GetDefaultWeight();							//Gets the default weight of the scheduler
	int GetLastPacketTime();						//Gets the time of the last packet that was read
	int GetMapSize();								//Gets the map size
	int Get_CurrNumOfPkts();						//Gets the current total number of packets
	long int GetSchTime();							//Gets the current time of the scheduler
	string GetInputFile();							//Gets the path of the input file
	string GetOutputFile();							//Gets the path of the output file
	string GetSchedulerType();						//Gets the scheduler type
	Packet GetLastPacket();							//Gets the last packet that was read
	vector<string> GetFlowsList();					//Gets the flows list

	//Setters
	void SetSchTime(int time);						//Sets the time of the scheduler.
	void SetCurrNumOfPkts(int value);				//Sets the new value of the current total number of packets
	void SetItemInFlowsList(string element);		//Sets a new item in the flows list
	void SetLastPacket(Packet pkt);					//Sets the last packet
	void SetLastTimePacket(int lastTime);			//Sets the last time of the last packet

	//Utils functions
	void Scheduler::RR_handleFlow(string key);		//Performs RR (actually WRR) handling for a certain flow
	
	void Scheduler::DRR_handleFlow(string key);		//Performs DRR handling for a certain flow

	void Scheduler::ScheduleCurrPakts();			//Sends the right packets according to current call

	bool Scheduler::IsFlowInMap(string key);		//Checks if the given key is in the map

	//Adds the given packet to the queue in the key place in the map
	//In case the key doesn't exist, create one and insert the packet
	//In case the map size reached the MAXSIZE - skip this flow
	void AddPacketToMap(string key, Packet pkt);

	void AddLastPacket();							//Adds the last packet that was recorded to the map

	void ReadUntilTimeChange();						//Reads until a new time arrives

	void Scheduler::RunAgain();						//Continues adding last packet and reading further lines

	void Run();										//Runs the routine of the scheduler
};

#endif