/***********************************
*            Includes              *
***********************************/
#include "Scheduler.h"
#include "schUtils.h"

/***********************************
*             Globals	           *
***********************************/
int LastIndexFlowList = 0;
ifstream inputReader;		//The reader for the input file
ofstream outputWriter;		//The writer for the output file


/***********************************
*             Functions	           *
***********************************/

//Constructor
Scheduler::Scheduler()
{
}
Scheduler::Scheduler(int quantom, int defaultWeight, int lastTimePacket,
	string SchedulerType, string inputFile, string outputFile)
{
	_quantum = quantom;
	_defaultWeight = defaultWeight; // when do we use it?
	_SchedulerType = SchedulerType;
	_inputFile = inputFile;
	_outputFile = outputFile;
	_lastPacketTime = lastTimePacket;
	_schTime = 0;
	_currNumOfPkts = 0;
	_startOver = false;
}

//Getters
string Scheduler::GetInputFile()
{
	return _inputFile;
}
string Scheduler::GetOutputFile()
{
	return _outputFile;
}
int Scheduler::GetQuantum()
{
	return _quantum;
}
int Scheduler::GetDefaultWeight()
{
	return _defaultWeight;
}
string Scheduler::GetSchedulerType()
{
	return _SchedulerType;
}
int Scheduler::GetLastPacketTime()
{
	return _lastPacketTime;
}
int Scheduler::GetMapSize()
{
	return _schedulerHashMap.size();
}
int Scheduler::Get_CurrNumOfPkts()
{
	return _currNumOfPkts;
}
long int Scheduler::GetSchTime()
{
	return _schTime;
}
Packet Scheduler::GetLastPacket()
{
	return _lastPacket;
}
vector<string> Scheduler::GetFlowsList()
{
	return _flowsList;
}

//Setters
void Scheduler::SetSchTime(int time)
{
	_schTime = time;
}

void Scheduler::SetCurrNumOfPkts(int value)
{
	_currNumOfPkts = value;
}

void Scheduler::SetStartOver(bool value)
{
	_startOver = value;
}

void Scheduler::SetItemInFlowsList(string element)
{
	_flowsList.push_back(element);
}

void Scheduler::SetLastPacket(Packet pkt)
{
	_lastPacket = pkt;
}

void Scheduler::SetLastTimePacket(int lastTime)
{
	_lastPacketTime = lastTime;
}

//General
bool Scheduler::IsFlowInMap(string key)
{
	if (_schedulerHashMap.find(key) != _schedulerHashMap.end())
	{
		return true;
	}

	return false;
}

bool Scheduler::IsStartOver()
{
	return _startOver;
}

void Scheduler::AddPacketToMap(string key, Packet pkt)
{
	if (GetMapSize() == MAX_SIZE)
	{
		return;
	}
 
	int pktTime = pkt.packet_GetTime();

	if (pktTime > GetSchTime())
	{
		SetSchTime(pktTime);
	}

	if (!IsFlowInMap(key))
	{
		Flow newFlow(pkt.packet_GetWeight(), 0, 0);
		SetItemInFlowsList(key);								//Append to RR ordered list
		_schedulerHashMap[key] = newFlow;						//Add to hash map
		_schedulerHashMap[key].flow_InsertToQueue(pkt);
		SetCurrNumOfPkts(Get_CurrNumOfPkts() + 1);
		return;
	}

	_schedulerHashMap[key].flow_InsertToQueue(pkt);
	SetCurrNumOfPkts(Get_CurrNumOfPkts() + 1);
}

void Scheduler::AddLastPacket()
{
	if (GetLastPacketTime() == -1)
	{
		return;
	}
	Packet lastPacket = GetLastPacket();
	string packetKey = GeneratePacketKey(lastPacket);
	AddPacketToMap(packetKey, lastPacket);
}

string Scheduler::ArrangePacketMsg(Packet pkt, int time)
{
	string resultMsg = std::to_string(time) + ": " + std::to_string(pkt.packet_GetID());

	return resultMsg;
}

void Scheduler::WriteToOutput(string pktMsg)
{
	outputWriter.open(_outputFile, ios::app);
	outputWriter << pktMsg + "\n";
	outputWriter.close();
}

void Scheduler::ReadUntilTimeChange()
{
	//Declarations
	string currLine, packetKey;

	//Program flow
	while (!inputReader.eof()) {
		while (getline(inputReader, currLine))
		{
			Packet currlinePacket = LineToPacket(currLine);
			if (currlinePacket.packet_GetWeight() == -1)
			{
				currlinePacket.packet_SetWeight(GetDefaultWeight());
			}
			packetKey = GeneratePacketKey(currlinePacket);

			//Check if the time of the new packet is different from the previous one
			if (GetLastPacketTime() != currlinePacket.packet_GetTime())
			{
				//Packet with new time has arrived
				if (GetLastPacketTime() != -1)
				{
					SetLastTimePacket(currlinePacket.packet_GetTime());
					SetLastPacket(currlinePacket);
					if (GetLastPacketTime() > GetSchTime())
					{
						return;
					}
				}
				else
				{
					SetLastTimePacket(currlinePacket.packet_GetTime());
				}
			}

			AddPacketToMap(packetKey, currlinePacket);
		}
	}
}

void Scheduler::Run()
{
	inputReader.open(GetInputFile());	//Opening the input file for reading

	while (!inputReader.eof())
	{
		AddLastPacket();				//Add the last packet that was already read in the last time.
		ReadUntilTimeChange();			//Read lines from the input file until a new time has been found
		ScheduleCurrPakts();
	}
}

void Scheduler::RunAgain()
{
	if (GetLastPacketTime() > GetSchTime()) 
	{
		return; 
	}
	if (!inputReader.eof()) {
		AddLastPacket();
		ReadUntilTimeChange();
	}
}

void Scheduler::RR_handleFlow(string key)
{
	Packet pkt;
	string msg;
	int currTime, packetLen;
	int flowWeight = _schedulerHashMap[key].flow_GetWeight();
	int flowDynamicWeight = flowWeight;
	Queue q = _schedulerHashMap[key].flow_GetQueue();

	while (!q.empty() && (flowDynamicWeight > 0))
	{
		pkt = q.front();
		if (pkt.packet_GetTime() > GetSchTime())
		{
			return;
		}

		packetLen = pkt.packet_GetLength();
		currTime = GetSchTime();
		msg = ArrangePacketMsg(pkt, currTime);
		WriteToOutput(msg);

		_schedulerHashMap[key].flow_PopFromQueue();
		q = _schedulerHashMap[key].flow_GetQueue();

		SetSchTime(currTime + packetLen);
		flowDynamicWeight--;
		SetCurrNumOfPkts(Get_CurrNumOfPkts() - 1);
		RunAgain();
		q = _schedulerHashMap[key].flow_GetQueue();
	}
}

void Scheduler::DRR_handleFlow(string key)
{
	Packet pkt;
	string msg;
	int currTime, packetLen;
	int flowWeight = _schedulerHashMap[key].flow_GetWeight();
	int flowCredit = _schedulerHashMap[key].flow_GetCredit();

	Queue q = _schedulerHashMap[key].flow_GetQueue();
	if (q.empty())
	{
		return;
	}

	pkt = q.front();
	packetLen = pkt.packet_GetLength();

	while (!q.empty())
	{
		pkt = q.front();
		packetLen = pkt.packet_GetLength();
		if (pkt.packet_GetTime() > GetSchTime())
		{
			return;
		}
		if (flowCredit < packetLen)
		{
			break;
		}
		
		currTime = GetSchTime();
		msg = ArrangePacketMsg(pkt, currTime);
		WriteToOutput(msg);

		_schedulerHashMap[key].flow_PopFromQueue();
		q = _schedulerHashMap[key].flow_GetQueue();

		flowCredit -= packetLen;
		_schedulerHashMap[key].flow_SetCredit(flowCredit);
		SetSchTime(currTime + packetLen);
		SetCurrNumOfPkts(Get_CurrNumOfPkts() - 1);
		RunAgain();
		q = _schedulerHashMap[key].flow_GetQueue();
	}

	if (q.empty())
	{
		_schedulerHashMap[key].flow_SetCredit(0);
	}
	else
	{
		int tmpCredit = (_schedulerHashMap[key].flow_GetWeight()) * (GetQuantum());
		_schedulerHashMap[key].flow_IncCredit(tmpCredit);
	}
}

void Scheduler::ScheduleCurrPakts()
{
	vector<string> RR_Flows_list = GetFlowsList();

	while (Get_CurrNumOfPkts() > 0)
	{
		LastIndexFlowList = 0;
		while (LastIndexFlowList < (RR_Flows_list.size()))
		{
			if (GetSchedulerType().compare("RR") == 0)
			{
				RR_handleFlow(RR_Flows_list[LastIndexFlowList]);
			}
			else
			{
				DRR_handleFlow(RR_Flows_list[LastIndexFlowList]);
			}

			RR_Flows_list = GetFlowsList();
			LastIndexFlowList++;
		}
	}
}
