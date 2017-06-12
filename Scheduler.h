#ifndef Scheduler_H
#define Scheduler_H

/***********************************
*            Includes              *
***********************************/

#include "Flow.h"
using namespace std;

/*************************************************************************
* Class Scheduler declaration:											 *
*																		 *
* The class defines a packet with the following members:				 *
* 1) _quantum : The quantum number for all the packets (size in bytes).	 *
* 2) _defaultWeight: The default weight of a flow with packets.			 *
* 3) _SchedulerType: The type of scheduler (false for RR, true for DRR). *
* 4) _inputFile: Holds the path of the input file.						 *
* 5) _outputFile: Holds the path of the output file.					 *
* 6) _schedulerHashMap: //Holds the keys and the flows					 *
*																		 *
* Description:															 *
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
	Scheduler();
	Scheduler(int quantom, int defaultWeight, int lastTimePacket,
		string SchedulerType, string inputFile, string outputFile);

	//Getters
	string GetInputFile();
	string GetOutputFile();
	int GetQuantum();
	int GetDefaultWeight();
	string GetSchedulerType();
	int GetLastPacketTime();
	int GetMapSize();
	int Get_CurrNumOfPkts();
	long int GetSchTime();
	bool IsFlowInMap(string key);
	Packet GetLastPacket();
	vector<string> GetFlowsList();

	//Setters
	void SetSchTime(int time);
	void SetCurrNumOfPkts(int value);
	void SetItemInFlowsList(string element);

	/****** Utils functions ******/

	//Adds the given packet to the queue in the key place in the map
	//In case the key doesn't exist, create one and insert the packet
	//In case the map size reached the MAXSIZE - skip this flow
	void AddPacketToMap(string key, Packet pkt);

	//Adds the last packet that was recorded to the map
	void AddLastPacket();

	//Sets the last packet
	void SetLastPacket(Packet pkt);

	//Sets the last time of the last packet
	void SetLastTimePacket(int lastTime);

	//Writes the report of the cuurent sent packet
	void Scheduler::WriteToOutput(string pktMsg);

	//Prepares packet for writing
	string Scheduler::ArrangePacketMsg(Packet pkt, int time);

	//Performs RR (actually WRR) handling for a certain flow
	void Scheduler::RR_handleFlow(string key);
	
	//Performs DRR handling for a certain flow
	void Scheduler::DRR_handleFlow(string key);

	//Sends the right packets according to current call
	void Scheduler::ScheduleCurrPakts();

	//Continues adding last packet and reading further lines
	void Scheduler::RunAgain();

	//
	void ReadUntilTimeChange();

	//
	void Run();
};

#endif