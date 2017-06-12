/***********************************
*            Includes              *
***********************************/
#include "SchedulerUtils.h"


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

void WriteToOutput(ofstream& outputWriter, string pktMsg, string outputFile)
{
	outputWriter.open(outputFile, ios::app);
	outputWriter << pktMsg + "\n";
	outputWriter.close();
}

string ArrangePacketMsg(Packet pkt, int time)
{
	string resultMsg = std::to_string(time) + ": " + std::to_string(pkt.packet_GetID());

	return resultMsg;
}