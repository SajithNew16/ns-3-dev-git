#pragma once
#include "TrustTable.h"
#include "TrustTableEntry.h"
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
	void identifyTrustLevel(TrustTable* trustTable);
	double calculateReductionFactor(int interactions, double globalTrust);
	double recalculateIndirectTrust(double indirectTrust, double reductionFactor);
	void SetBackupTable (BackupTable* backupTable);
	void calculateReductionFactor(TrustTableEntry trustTableEntry);
	void calculateReductionFactorForNeibors(TrustTable* trustTable, TrustTableEntry trustTableEntry);
	~TrustLevelClassifier();
};

}
}
