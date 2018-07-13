//#include "stdafx.h"
#include "RecommendationTable.h"
#include <string>
#include <iostream>
#include <vector>

namespace ns3
{
namespace trust
{


RecommendationTable::RecommendationTable()
{
	columnSeperator = " | ";
}


void RecommendationTable::addRecommendationTableEntry(RecommendationTableEntry entry)
{
	recommendationTableRecords.push_back(entry);
}

void RecommendationTable::removeRecommendationTableEntry(RecommendationTableEntry entry)
{
	recommendationTableRecords.pop_back(); //need to change this
}

std::vector<RecommendationTableEntry>& RecommendationTable::getRecommendationTableEntries()
{
	return recommendationTableRecords;
}

std::vector<RecommendationTableEntry>& RecommendationTable::getRecommendedNodeEntries(Ipv4Address nodeId)
{
	// TODO: return the recommended node entry vector
	std::vector<RecommendationTableEntry>& test = this->getRecommendationTableEntries();

	// TODO: need to have a logic here.
	for (std::vector<RecommendationTableEntry>::iterator it = recommendationTableRecords.begin(); it != recommendationTableRecords.end(); it++)
	{
		if (it->getneighborNodeId() == nodeId) {
			test.push_back(*it);
		}
	}

	//need to do something(exception handling) for in all the calling functions
	return test;
}

void RecommendationTable::setRecommendationTable(std::vector<RecommendationTableEntry> newRecommendationTable)
{
	this->recommendationTableRecords = newRecommendationTable;
}

void RecommendationTable::printTable()
{
	/*std::cout << "| " << "Neighbor Node" << columnSeperator << "Recommending Nodes" << columnSeperator << "Recommending Value(DT)" << columnSeperator <<"Recommending Value(GT)" << "\t" << columnSeperator << "Matuarity Level" << "\t" << columnSeperator << "Blacklist" << std::endl;

	for (std::vector<RecommendationTableEntry>::iterator it = recommendationTableRecords.begin(); it != recommendationTableRecords.end(); it++)
	{
	std::cout << "| " << it->getneighborNodeId() << "\t" << columnSeperator << it->getRecommendingNodes() << "\t\t" << columnSeperator << it->getrecValue_DT() << "\t\t" << columnSeperator << it->getrecValue_GT() << "\t\t" << columnSeperator << it->getMaturityLevel() << "\t\t" << columnSeperator << it->getBlacklistStatus() << "\t\t" << columnSeperator << std::endl;
	}*/

	uint n = 5;
  //printing only last 5 records
  if (recommendationTableRecords.size () >= n)
    {
      std::vector<RecommendationTableEntry> y (recommendationTableRecords.end () - n,
                                       recommendationTableRecords.end ());
      std::cout << ">>>>>>>>>>>>>> Last 5 records of Recommendation Table <<<<<<<<<<<<<<<" << std::endl;
      std::cout << "| " << "Node" << columnSeperator << "Trust Value" << columnSeperator << "Time Duration" << columnSeperator
          << "Analyzed Result" << std::endl;
      for (std::vector<RecommendationTableEntry>::iterator it = y.begin (); it != y.end (); it++)
        {
          std::cout << "| " << it->getneighborNodeId() << "\t" << columnSeperator << it->getRecommendingNodes() << "\t\t" << columnSeperator << it->getrecValue_DT() << "\t\t" << columnSeperator << it->getrecValue_GT() << "\t\t" << columnSeperator << it->getMaturityLevel() << "\t\t" << columnSeperator << it->getBlacklistStatus() << "\t\t" << columnSeperator << std::endl;
        }
    }
  //printing all the records for that moment if records count get less than 5
  else
    {
      std::cout << "| " << "Node" << columnSeperator << "Trust Value" << columnSeperator << "Time Duration" << columnSeperator
          << "Analyzed Result" << std::endl;
      for (std::vector<RecommendationTableEntry>::iterator it = recommendationTableRecords.begin ();
          it != recommendationTableRecords.end (); it++)
        {
          std::cout << "| " << it->getneighborNodeId() << "\t" << columnSeperator << it->getRecommendingNodes() << "\t\t" << columnSeperator << it->getrecValue_DT() << "\t\t" << columnSeperator << it->getrecValue_GT() << "\t\t" << columnSeperator << it->getMaturityLevel() << "\t\t" << columnSeperator << it->getBlacklistStatus() << "\t\t" << columnSeperator << std::endl;
        }
    }
}



RecommendationTable::~RecommendationTable()
{
}

}
}
