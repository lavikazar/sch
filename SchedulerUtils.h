#ifndef SchedulerUtils_H
#define SchedulerUtils_H

/***********************************
*            Includes              *
***********************************/
#include "Generals.h"
#include "Packet.h"
using namespace std;

void SplitLineByDelimiter(vector<string>* packetArgs, string line, char* delim);	//Splits string by a given delimeter

string GeneratePacketKey(Packet pkt);	//Generate key for a given packet. The genrated key is: (sourceAddressIP + soucePot + destAddressIP + destPort)

Packet LineToPacket(string line);	//Parses line to packet form

void WriteToOutput(ofstream& outputWriter, string pktMsg, string outputFile);	//Writes the report of the cuurent sent packet

string ArrangePacketMsg(Packet pkt, int time);	//Prepares packet for writing

#endif
