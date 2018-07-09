#include "DirTrustCal.h"
#include "TrustTableEntry.h"
#include <math.h>
#include <iostream>

namespace ns3
{
namespace trust
{

DirTrustCal::DirTrustCal()
{
}

void DirTrustCal::calculateDirectTrust(TrustTable *trustTable)
{
	std::vector<TrustTableEntry>& node_entry_vector = trustTable->getTrustTableEntries();

	for (std::vector<TrustTableEntry>::iterator it = node_entry_vector.begin(); it != node_entry_vector.end(); it++)
	{
		int rreq = it->getNoOfRREQ();
		int rply =  it->getNoOfRPLY();
		int hello =  it->getNoOfHELLO();
//		int err =  it->getNoOfERR();
		int sent =  it->getNoOfSentDataPackets();
		int received =  it->getNoOfReceivedDataPackets();
		int interactions = it->getInteractionCount();

		double cp = (rreq + rply + hello ) / 3.0;
		double dp = 0;

		if (received == 0) {
			dp = 0;
		}
		else{
			dp = sent / (double) received;
		}

		double dt = (cp + dp)*2/(double)( interactions);
		double value = round( dt * 100000.0 ) / 100000.0;

	//	double finaldt = calculateFinalDT(dt);

		it->setDirectTrust(value);
	}
}

double DirTrustCal::calculateFinalDT(double directTrustValue)
{
	//double scaledDT = 1 - (1 / pow(directTrustValue , 0.5));
	//return scaledDT;


	double scaledDT = 1 - (1 / pow(directTrustValue , 0.5));

	double value = round( scaledDT * 100000.0 ) / 100000.0;


	return value;

}

DirTrustCal::~DirTrustCal()
{
}

}
}
