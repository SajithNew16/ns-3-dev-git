//#include "stdafx.h"
#include "RecommendationTableEntry.h"
#include <string>

namespace ns3
{
namespace trust
{

RecommendationTableEntry::RecommendationTableEntry()
{
}

Ipv4Address RecommendationTableEntry::getneighborNodeId()
{
	return this->neighborNodeId;
}

Ipv4Address RecommendationTableEntry::getRecommendingNodes()
{
	return this->recommendingNode;
}

double RecommendationTableEntry::getMaturityLevel()
{
	return this->maturityLevel;
}
double RecommendationTableEntry::getrecValue_DT()
{
	return this->recommendationValue_DT;
}

double RecommendationTableEntry::getrecValue_GT()
{
	return this->recommendationValue_GT;
}

bool RecommendationTableEntry::getBlacklistStatus()
{
	return this->blackList;
}

void RecommendationTableEntry::setNeighborNodeId(Ipv4Address neighborNodeId)
{
	this->neighborNodeId = neighborNodeId;
}

void RecommendationTableEntry::setMaturityLevel(double maturityLevel)
{
	this->maturityLevel = maturityLevel;
}

void RecommendationTableEntry::setRecValue_DT(double recommendationValue_DT)
{
	this->recommendationValue_DT = recommendationValue_DT;
}


void RecommendationTableEntry::setRecValue_GT(double recommendationValue_GT)
{
	this->recommendationValue_GT = recommendationValue_GT;
}

void RecommendationTableEntry::setRecommendingNode(Ipv4Address recommendingNode)
{
	this->recommendingNode = recommendingNode;
}

void RecommendationTableEntry::blacklistNode(bool status)
{
	this->blackList = status;
}


/**
 * Method:    calculateMaturityLevel
 * Returns:   maturity level
 * Parameter: node
 */
void RecommendationTableEntry::calculateMaturityLevel(std::vector<TrustTableEntry> node_entry_list) {

	int i_p_node = 0;
	double i_all = 0;

	for (std::vector<TrustTableEntry>::iterator it = node_entry_list.begin();it != node_entry_list.end(); it++) {
		if(this->neighborNodeId == it->getDestinationNode()){
			 i_p_node = it->getInteractionCount();
		}

		i_all = i_all + it->getInteractionCount();
	}

	this->maturityLevel = i_p_node / i_all;
}


RecommendationTableEntry::~RecommendationTableEntry()
{
}

}
}
