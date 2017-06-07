/***********************************
*            Includes              *
***********************************/
#include "schUtils.h"

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