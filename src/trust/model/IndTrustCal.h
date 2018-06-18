#pragma once
#include "TrustTable.h"
#include "TrustTableEntry.h"
#include "TRRTable.h"
#include "TRRTableEntry.h"

namespace ns3
{
namespace trust
{

class IndTrustCal
{
private:
	TRRTable* trrTable;
	TrustTable* trustTable;
	uint trrRecFlag;
public:
	IndTrustCal();
	double* sendTRR(TrustTableEntry node, TrustTableEntry targetNode);
	double calculateWeight(TrustTableEntry node, TrustTableEntry targetNode);
	double calculateRNew(TrustTableEntry node ,TrustTableEntry targetNode);
	double* getDTGT(TrustTableEntry node, TrustTableEntry targetNode);
	double calculateMaturityLevel(TrustTableEntry node);
	void setTrustTable(TrustTable* trustTable);
	double calculateIndirectTrust(TrustTableEntry targetNode);
    ///This SetFlag method will call when TRR reply packet get received
	void SetTrrRecFlag (uint trrRecFlag);
	void SetTrrTable (TRRTable* trrTable);
	~IndTrustCal();
};

}
}

