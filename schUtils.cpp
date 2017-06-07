/***********************************
*            Includes              *
***********************************/
#include "schUtils.h"


/***********************************
*            Globals			   *
***********************************/

ifstream inputReader;		//The reader for the input file


/***********************************
*            Functions			   *
***********************************/

void SplitLineByDelimiter(vector<string>* packetArgs, string line, char* delim)
{
	 char *y = new char[line.length() + 1];
	 std::strcpy(y, line.c_str());

	 char* tmp = strtok(y, delim);
	 while (tmp != NULL)
	 {
		 packetArgs->push_back(tmp);
		 tmp = strtok(NULL, delim);
	 }

	 delete[] y;
 }

string GeneratePacketKey(Packet pkt)
{
	string res(pkt.packet_GetSourceAddr() + pkt.packet_GetSourcePort() + pkt.packet_GetDestAddr() + pkt.packet_GetDestPort());
	return res;
}

Packet LineToPacket(string line)
{
	int weight = -1;
	vector<string> packetArgs;
	SplitLineByDelimiter(&packetArgs, line, " "); // Splits the line by a 'Space' delimiter
	int size = packetArgs.size();
	if (size == 8)
	{
		weight = stoi(packetArgs[7]);
	}

	Packet resPacket(stol(packetArgs[0]), stol(packetArgs[1]), packetArgs[2], packetArgs[3], packetArgs[4], packetArgs[5], stoi(packetArgs[6]), weight);
	return resPacket;
}

void ComputeLcmWeight(int* lcm, int n1, int n2)
{
	int max;
	max = (n1 > n2) ? n1 : n2;

	do
	{
		if (max % n1 == 0 && max % n2 == 0)
		{
			*lcm = max;
			break;
		}
		else
			++max;
	} while (true);
}

void ReadUntilTimeChange(Scheduler* sch)
{
	//Declarations
	string currLine, packetKey;

	//Program flow
	while (getline(inputReader, currLine))
	{
		Packet currlinePacket = LineToPacket(currLine);
		if (currlinePacket.packet_GetWeight() == -1)
		{
			currlinePacket.packet_SetWeight(sch->GetDefaultWeight());
		}
		packetKey = GeneratePacketKey(currlinePacket);

		//Check if the time of the new packet is different from the previous one
		if (sch->GetLastTimePacket() != currlinePacket.packet_GetTime())
		{
			//Packet with new time has arrived
			if (sch->GetLastTimePacket() != -1)
			{
				sch->SetLastTimePacket(currlinePacket.packet_GetTime());
				sch->SetLastPacket(currlinePacket);
				return;
			}
			sch->SetLastTimePacket(currlinePacket.packet_GetTime());
		}

		sch->AddPacketToMap(packetKey, currlinePacket);
	}
}

void Run(Scheduler* sch)
{
	inputReader.open(sch->GetInputFile()); //Opening the input file for reading

	while (inputReader)
	{
		sch->AddLastPacket();			//Add the last packet that was already read in the last time.
		ReadUntilTimeChange(sch);	//Read lines from the input file until a new time has been found
		sch->ScheduleCurrPakts(sch);
	}
}