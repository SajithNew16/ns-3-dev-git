
#include "IndTrustCal.h"
#include "TrustTableEntry.h"
#include "DirTrustCal.h"
#include "NodeEntry.h"
#include "TestValueGenerator.h"
#include "TrustLevelClassifier.h"
#include "trust-routing-protocol.h"
#include "ns3/log.h"
#include <iostream>

namespace ns3 {
namespace trust {

IndTrustCal::IndTrustCal() {
}

/**
 * Method:    sendTRR
 * Returns:   rec
 * Parameter: node, targetNode
 */
double* IndTrustCal::sendTRR(TrustTableEntry node, TrustTableEntry targetNode) {

	double static rec[2];
	rec[0] = 0.5;
	rec[1] = 0.6;
	/*
	std::vector<RecommendationTableEntry> node_entry_list = this->recommendationTable->getRecommendationTableEntries();

	for (std::vector<RecommendationTableEntry>::iterator it = node_entry_list.begin();it != node_entry_list.end(); it++) {
		if (it->getneighborNodeId() != targetNode.getDestinationNode()) {
			rec[0] = targetNode.getDirectTrust();
			rec[1] = targetNode.getGlobalTrust();
			//to be completed
		}
	}
*/
	return rec;
}

/**
 * Method:    setTrustTable
 * Returns:
 * Parameter: trustTable
 */
void IndTrustCal::setTrustTable(TrustTable* trustTable) {
	this->trustTable = trustTable;
}

/**
 * Method:    setRecommendationTable
 * Returns:
 * Parameter: recommendationTable
 */
void IndTrustCal::setRecommendationTable(RecommendationTable* recommendationTable) {
	this->recommendationTable = recommendationTable;
}

/**
 * Method:    calculateWeight
 * Returns:   weight
 * Parameter: node, targetNode
 */
double IndTrustCal::calculateWeight(TrustTableEntry node, TrustTableEntry targetNode) {

	double r_new_nei_node = calculateRNew(node, targetNode);
	double r_new_all = 0;
	std::vector<TrustTableEntry> node_entry_list =
			this->trustTable->getTrustTableEntries();
	for (std::vector<TrustTableEntry>::iterator it = node_entry_list.begin();
			it != node_entry_list.end(); it++) {
		if (it->getDestinationNode() != targetNode.getDestinationNode()) {
			double r_new_node = calculateRNew(*it, targetNode);
			r_new_all = r_new_all + r_new_node;
		}
	}

	return r_new_nei_node / r_new_all;
}

/**
 * Method:    calculateRNew
 * Returns:   new Recommendation
 * Parameter: node, targetNode
 */
double IndTrustCal::calculateRNew(TrustTableEntry node, TrustTableEntry targetNode) {

	double mlevel = 0.0;
	double rNew = 0.0;
	std::vector<RecommendationTableEntry> node_entry_list = this->recommendationTable->getRecommendationTableEntries();

		for (std::vector<RecommendationTableEntry>::iterator it = node_entry_list.begin();it != node_entry_list.end(); it++) {
			if(it->getneighborNodeId() == node.getDestinationNode() && it->getRecommendingNodes() == targetNode.getDestinationNode()){
				this->recDT = it->getrecValue_DT();
				mlevel = it->getMaturityLevel();
				//std::cout << "mlevel---> " << mlevel  << std::endl;
			}
		}

	rNew = mlevel * (this->recDT);
//	std::cout << "rNew--> " << rNew << std::endl;

	if(rNew != 0){
	return rNew;
	}
	else{
		//initial default value
		return 0.066815;
	}

/*
	double mlevel = 0.7;	//temporary static value
	double* rec;
	rec = getDTGT(node, targetNode);

	return mlevel * rec[0];
	*/
}

/**
 * Method:    getDTGT
 * Returns:   rec
 * Parameter: node, targetNode
 */
double* IndTrustCal::getDTGT(TrustTableEntry node, TrustTableEntry targetNode) {

	double* rec;
	rec = sendTRR(node, targetNode);

	return rec;
}


/**
 * Method:    calculateIndirectTrust
 * Returns:   w_sum
 * Parameter: targetNode
 */
double IndTrustCal::calculateIndirectTrust(TrustTableEntry targetNode) {

	std::vector<TrustTableEntry> node_entry_list = this->trustTable->getTrustTableEntries();
	double w_sum = 0;

	for (std::vector<TrustTableEntry>::iterator it = node_entry_list.begin();it != node_entry_list.end(); it++) {
		if (it->getDestinationNode() != targetNode.getDestinationNode()) {
			double w = calculateWeight(*it, targetNode);
			double r_new_nei_node = calculateRNew(*it, targetNode);
			double *rec;
			rec = getDTGT(*it, targetNode);
			double cal_w_term = w * (r_new_nei_node * rec[1]);

//			std::cout << "r_new_nei_node="<< r_new_nei_node << std::endl;
//			std::cout << "recGT" << this->recGT << std::endl;

//			double cal_w_term = w * (r_new_nei_node * (this->recGT));
			w_sum = w_sum + cal_w_term;
		}
	}
	return w_sum;
}

IndTrustCal::~IndTrustCal() {
}

}
}
