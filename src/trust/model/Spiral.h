#pragma once
#include "TrustTable.h"
#include "BackupTable.h"

namespace ns3
{
namespace trust
{
class Spiral
{
public:
	Spiral ();
	void addMaliciousCategory(double* past_global_trust_range, TrustTable* trustTable);
	double* GetMinMaxTrust (BackupTable* backupTable, TrustTableEntry trustTableEntry);
	~Spiral ();
};
}
}

