#pragma once
#include <vector>
#include <string>
#include "TrustTableEntry.h"
#include "ns3/ipv4-address.h"

namespace ns3
{
namespace trust
{

class TrustTable
{
private:
  std::string columnSeperator;
	std::vector<TrustTableEntry> trustTableRecords;
public:
  TrustTable();
	void addTrustTableEntry(TrustTableEntry entry);
	void removeTrustTableEntry(TrustTableEntry entry);
	void removeTrustTableEntryByIndex (TrustTableEntry entry,int index);
	TrustTableEntry* getTrustTableEntryByNodeId(Ipv4Address nodeId);
	std::vector<TrustTableEntry> getBlacklistedTrustTableEntries();
	std::vector<TrustTableEntry>& getTrustTableEntries();
	void setTrustTable(std::vector<TrustTableEntry> newTrustTable);
	void incrementAllHelloPacketsCount();
	void printTable();
	~TrustTable();
};

}
}
