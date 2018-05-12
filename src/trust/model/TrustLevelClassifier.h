#pragma once
#include "TrustTable.h"

namespace ns3
{
namespace trust
{

class TrustLevelClassifier
{
public:
	TrustLevelClassifier();
	void identifyTrustLevel(TrustTable* trustTable);
	double calculateReductionFactor(int interactions, double globalTrust);
	double recalculateIndirectTrust(double indirectTrust, double reductionFactor);
	~TrustLevelClassifier();
};

}
}
