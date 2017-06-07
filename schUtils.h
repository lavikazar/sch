#ifndef schUtils_H
#define schUtils_H

/***********************************
*            Includes              *
***********************************/
#include "Generals.h"
#include "Packet.h"
using namespace std;

//Splits string by a given delimeter
void SplitLineByDelimiter(vector<string>* packetArgs, string line, char* delim);

//Generate key for a given packet. The genrated key is: (sourceAddressIP + soucePot + destAddressIP + destPort)
string GeneratePacketKey(Packet pkt);

//Parses line to packet form
Packet LineToPacket(string line);

//Calculate the lacm of two numbers
void ComputeLcmWeight(int* lcm, int n1, int n2);
#endif
