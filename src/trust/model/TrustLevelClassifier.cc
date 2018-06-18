#include "TrustLevelClassifier.h"
#include "RecommendationTableEntry.h"
#include "IndTrustCal.h"
#include "TrustTableEntry.h"
#include "DirTrustCal.h"
#include "Spiral.h"
#include "TrustInfoReceiver.h"
#include <vector>
#include <iostream>

namespace ns3
{
namespace trust
{

TrustLevelClassifier::TrustLevelClassifier()
{
}


void TrustLevelClassifier::identifyTrustLevel(TrustTable* trustTable)
{
	double Threshold_trust = 0.5;
	double Threshold_trustWorthy = 0.8;
	double Threshold_selfish = 0.3;

	std::vector<TrustTableEntry>& node_entry_vector = trustTable->getTrustTableEntries();
	double node_GT;

	for (std::vector<TrustTableEntry>::iterator it = node_entry_vector.begin(); it != node_entry_vector.end(); it++) {
		node_GT = it->getGlobalTrust();
		if (node_GT > Threshold_trust)
		{
			if (node_GT > Threshold_trustWorthy)
			{
				it->setTrustLevel(1);
			}
			else
			{
				it->setTrustLevel(2);
			}
		}
		else
		{
			if (node_GT > Threshold_selfish)
			{
				it->setTrustLevel(3);

				//reduce trust here
			}
			else
			  {
				//call spiral model method
				Spiral spiralModel;
				double *past_global_trust_range;
				past_global_trust_range = spiralModel.GetMinMaxTrust (this->backupTable);
				TrustInfoReceiver trustInfoReceiver;
				if (past_global_trust_range[0] <= node_GT && past_global_trust_range[1] >= node_GT)
				  {
					it->setTrustLevel(4);
					if (this->afterExecuteFirstFlag == 2)
					  {
					    trustTable->removeTrustTableEntry(*it);
					    it->setTrustLevel(4);
					    trustInfoReceiver.sendMaliciousBroadcast(it->getDestinationNode());
					  }
				  }
				else
				  {
					it->setTrustLevel(5);
					it->setBlacklist(true);
					//neighbour_node_list = getRecommendedNodes(inputNode);
					//reduce_factor = calculateReduceFactor(input_node);
					//for (int i = 0; i<neighbour_node_array.length(); i++) {
					//  recalculateIndirectTrust(neighbour_node, reduce_factor);
					//	updateGlobalTrust(node);
					//}

					//Broadcast to the others that this is a collaborative malicious nodes
					trustInfoReceiver.sendMaliciousBroadcast(it->getDestinationNode());

					//broadcastToNeighbors(input_node,"Collaborative malicious node");
					// GOTO Identifying_trust_levels
				  }
//				model.addMaliciousCategory(past_global_trust_range, trustTable);
			  }
		}
	}
}

void
TrustLevelClassifier::SetBackupTable (BackupTable* backupTable) {
	this->backupTable = backupTable;
}

double TrustLevelClassifier::calculateReductionFactor(int interactions, double globalTrust)
{
	double reductionFactor;
	reductionFactor = globalTrust / interactions;

	return reductionFactor;
}


double TrustLevelClassifier::recalculateIndirectTrust(double indirectTrust, double reductionFactor)
{
	indirectTrust = indirectTrust - reductionFactor;
	//update global trust
	return indirectTrust;
}

void
TrustLevelClassifier::SetAfterExecuteFirstFlag (uint afterExecuteFirstFlag)
{
	this->afterExecuteFirstFlag = afterExecuteFirstFlag;
}

TrustLevelClassifier::~TrustLevelClassifier()
{
}

}
}
