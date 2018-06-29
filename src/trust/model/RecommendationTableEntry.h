#pragma once
#include <string>
#include <vector>
#include <list>
#include "NodeEntry.h"
#include "ns3/ipv4-address.h"
#include "TrustTable.h"
#include "TrustTableEntry.h"


namespace ns3
{
namespace trust
{


class RecommendationTableEntry
{

private:
	Ipv4Address neighborNodeId;
	Ipv4Address recommendingNode;
	double maturityLevel;
	bool blackList;
	double recommendationValue_GT;
	double recommendationValue_DT;

public:
	Ipv4Address getneighborNodeId();
	void setNeighborNodeId(Ipv4Address neighborNodeId);
	Ipv4Address getRecommendingNodes();
	double getrecValue_DT();
	double getrecValue_GT();
	double getMaturityLevel();
	bool getBlacklistStatus();
	void setRecValue_DT(double recommendationValue_DT);
	void setRecValue_GT(double recommendationValue_GT);
	void setRecommendingNode(Ipv4Address recommendingNode);
	void setMaturityLevel(double maturityLevel);
	void calculateMaturityLevel(std::vector<TrustTableEntry> node_entry_list);
	void blacklistNode(bool status);
	RecommendationTableEntry();
	~RecommendationTableEntry();
};

}
}
