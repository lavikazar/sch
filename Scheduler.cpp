/***********************************
*            Includes              *
***********************************/
#include "Scheduler.h"
#include "schUtils.h"

/***********************************
*             Globals	           *
***********************************/

ofstream outputWriter;			//The writer for the output file


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
	_lastTimePacket = lastTimePacket;
	_schTime = 0;
	_currNumOfPkts = 0;
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
int Scheduler::GetLastTimePacket()
{
	return _lastTimePacket;
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
list<string> Scheduler::GetFlowsList()
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
	int credit = GetQuantum() * pkt.packet_GetWeight();

	if (pktTime >= GetSchTime())
	{
		SetSchTime(pktTime);
	}
	else
	{
		SetStartOver(true);
	}

	if (!IsFlowInMap(key))
	{
		//ComputeLcmWeight(&lcm, pkt.packet_GetWeight(), GetLcm());	//Compute the new lcmWeight
		Flow newFlow(pkt.packet_GetWeight(), credit, 0);
		list<string> RR_Flows_list = GetFlowsList();
		RR_Flows_list.push_back(key);								//Append to RR ordered list
		_schedulerHashMap[key] = newFlow;							//Add to hash map
		_schedulerHashMap[key].flow_InsertToQueue(pkt);
		return;
	}

	_schedulerHashMap[key].flow_IncCredit(credit);
	_schedulerHashMap[key].flow_InsertToQueue(pkt);
	SetCurrNumOfPkts(Get_CurrNumOfPkts() + 1);
}

void Scheduler::AddLastPacket()
{
	if (GetLastTimePacket() == -1)
	{
		return;
	}
	Packet lastPacket = GetLastPacket();
	string packetKey = GeneratePacketKey(lastPacket);
	AddPacketToMap(packetKey, lastPacket);
}

void Scheduler::SetLastPacket(Packet pkt)
{
	_lastPacket = pkt;
}

void Scheduler::SetLastTimePacket(int lastTime)
{
	_lastTimePacket = lastTime;
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

void Scheduler::RR_handleFlow(string key)
{
	Packet pkt;
	string msg;
	int currTime, packetLen;
	int flowWeight = _schedulerHashMap[key].flow_GetWeight();
	//int numOfSends = GetLcm() / flowWeight;
	int flowDynamicWeight = flowWeight;
	Queue q = _schedulerHashMap[key].flow_GetQueue();

	while (!q.empty() && (flowDynamicWeight > 0))
	{
		pkt = q.front();
		packetLen = pkt.packet_GetLength();
		currTime = GetSchTime();
		msg = ArrangePacketMsg(pkt, currTime);
		WriteToOutput(msg);
		q.pop();
		SetSchTime(currTime + packetLen);
		flowDynamicWeight--;
		SetCurrNumOfPkts(Get_CurrNumOfPkts() - 1);
		ReadUntilTimeChange();
	}
}

void Scheduler::DRR_handleFlow(string key)
{
	Packet pkt;
	string msg;
	int currTime;
	int	packetLen = pkt.packet_GetLength();
	int flowWeight = _schedulerHashMap[key].flow_GetWeight();
	int flowCredit = _schedulerHashMap[key].flow_GetCredit();
	Queue q = _schedulerHashMap[key].flow_GetQueue();

	while (!q.empty() && (flowCredit > packetLen))
	{
		pkt = q.front();
		packetLen = pkt.packet_GetLength();
		currTime = GetSchTime();
		msg = ArrangePacketMsg(pkt, currTime);
		WriteToOutput(msg);
		flowCredit -= packetLen;
		q.pop();
		SetSchTime(currTime + packetLen);
		SetCurrNumOfPkts(Get_CurrNumOfPkts() - 1);
		ReadUntilTimeChange();
	}

	if (q.empty())
	{
		_schedulerHashMap[key].flow_SetCredit(0);
	}
	else
	{
		_schedulerHashMap[key].flow_IncCredit(flowCredit);
	}
}

void Scheduler::ScheduleCurrPakts()
{
	std::list<string>::iterator it;
	list<string> RR_Flows_list = GetFlowsList();

	while (Get_CurrNumOfPkts() > 0)
	{
		for (it = RR_Flows_list.begin(); it != RR_Flows_list.end(); ++it)
		{
			int prevNumOfPackets = Get_CurrNumOfPkts();

			if (_SchedulerType.compare("RR"))
			{
				RR_handleFlow(*it);
			}
			else
			{
				DRR_handleFlow(*it);
			}

			if (prevNumOfPackets > Get_CurrNumOfPkts()) //Case SCH time has chenged (some packets were sent)
			{
				ReadUntilTimeChange();
				if (IsStartOver)
				{
					it = RR_Flows_list.begin(); //Start iterating from the beginning
				}
			}
		}
	}
}