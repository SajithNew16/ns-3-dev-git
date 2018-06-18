#pragma once
#include "TrustTable.h"
#include "BackupTable.h"

namespace ns3
{
namespace trust
{

class TrustLevelClassifier
{
private:
	BackupTable* backupTable;
public:
	TrustLevelClassifier();
	void SetBackupTable (BackupTable* backupTable);
	void identifyTrustLevel(TrustTable* trustTable);
	double calculateReductionFactor(int interactions, double globalTrust);
	double recalculateIndirectTrust(double indirectTrust, double reductionFactor);
	~TrustLevelClassifier();
};

}
}
