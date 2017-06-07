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

/***********************************
*          Main program     	   *
***********************************/


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
