/***********************************
*            Includes              *
***********************************/

#include "schUtils.h"
#include "Scheduler.h"
using namespace std;

/***********************************
*            Globals			   *
***********************************/

Scheduler* sch;				//Holds the scheduler
ifstream inputReader;		//The reader for the input file

/***********************************
*      Main program functions	   *
***********************************/

//Reading until a new time is being read
void ReadUntilTimeChange()
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

//Runs the routine of the scheduler
void Run(Scheduler* sch)
{
	inputReader.open(sch->GetInputFile()); //Opening the input file for reading

	while (inputReader)
	{
		sch->AddLastPacket();			//Add the last packet that was already read in the last time.
		ReadUntilTimeChange();	//Read lines from the input file until a new time has been found
		sch->ScheduleCurrPakts();
	}
}


/***********************************
*            Main Program          *
***********************************/

int main(int argc, char* argv[])
{
	if (argc != 6)
	{
		cout << "Invalid number of arguments" << endl;
		return -1;
	}

	string inputFile = argv[2], outputFile = argv[3];
	string schedulerType = "RR";
	int defaultWeight = stoi(argv[4]), quantum = stoi(argv[5]);
	
	if (argv[1] == "DRR")
	{
		schedulerType = "DRR";
	}

	ofstream f = ofstream(outputFile, ios::trunc);	//Truncate the output file to start fresh new file
	f.close();										//Closes the output file

	//Initialize the scheduler
	sch = new Scheduler(quantum, defaultWeight, -1, schedulerType, inputFile, outputFile);
	
	//Run routine
	Run(sch);
	
	return 0;
}
