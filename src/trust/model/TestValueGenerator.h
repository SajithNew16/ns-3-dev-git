#pragma once
#include "BackupTable.h"
#include "TrustTable.h"
#include "RecommendationTable.h"


namespace ns3
{
namespace trust
{

class TestValueGenerator
{
public:
	TestValueGenerator();
	static TrustTable* getDummyTrustTable();
	static TrustTable* getDummyDirTrustTable();
	static RecommendationTable* getDummyRecommendationTableByTrustTable(TrustTable* trustTable);
	~TestValueGenerator();
};

}
}
