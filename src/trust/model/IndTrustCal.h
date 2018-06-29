#pragma once
#include "TrustTable.h"
#include "TrustTableEntry.h"
#include "RecommendationTable.h"
#include "RecommendationTableEntry.h"

namespace ns3
{
namespace trust
{

class IndTrustCal
{
private:
	TrustTable* trustTable;
	RecommendationTable* recommendationTable;
	double recGT;
	double recDT;

public:
	IndTrustCal();
	double* sendTRR(TrustTableEntry node, TrustTableEntry targetNode);
	double calculateWeight(TrustTableEntry node, TrustTableEntry targetNode);
	double calculateRNew(TrustTableEntry node ,TrustTableEntry targetNode);
	double* getDTGT(TrustTableEntry node, TrustTableEntry targetNode);
	double calculateMaturityLevel(TrustTableEntry node);
	void setTrustTable(TrustTable* trustTable);
	void setRecommendationTable(RecommendationTable* recommendationTable);
	double calculateIndirectTrust(TrustTableEntry targetNode);
	~IndTrustCal();
};

}
}
