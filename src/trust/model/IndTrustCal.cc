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

IndTrustCal::IndTrustCal ()
{
}

/**
 * Method:    sendTRR
 * Returns:   rec
 * Parameter: node, targetNode
 */
double* IndTrustCal::sendTRR (TrustTableEntry node,
                              TrustTableEntry targetNode)
{

  double static rec[2];
  rec[0] = 1.0;
  rec[1] = 1.0;

  std::vector<RecommendationTableEntry> node_entry_list = this->recommendationTable->getRecommendationTableEntries ();

  for (std::vector<RecommendationTableEntry>::iterator it = node_entry_list.begin (); it != node_entry_list.end (); it++)
    {
      if (it->getneighborNodeId () != targetNode.getDestinationNode ())
        {
          rec[0] = it->getrecValue_DT ();
          rec[1] = it->getrecValue_GT ();
        }
    }
  return rec;
}

/**
 * Method:    setTrustTable
 * Returns:
 * Parameter: trustTable
 */
void IndTrustCal::setTrustTable (TrustTable* trustTable)
{
  this->trustTable = trustTable;
}

/**
 * Method:    setRecommendationTable
 * Returns:
 * Parameter: recommendationTable
 */
void IndTrustCal::setRecommendationTable (RecommendationTable* recommendationTable)
{
  this->recommendationTable = recommendationTable;
}

/**
 * Method:    calculateWeight
 * Returns:   weight
 * Parameter: node, targetNode
 */
double IndTrustCal::calculateWeight (TrustTableEntry node,
                                     TrustTableEntry targetNode)
{

  double r_new_nei_node = calculateRNew (node,
                                         targetNode);
  double r_new_all = 0.0;
  double weight = 0.0;
  std::vector<TrustTableEntry> node_entry_list = this->trustTable->getTrustTableEntries ();
  for (std::vector<TrustTableEntry>::iterator it = node_entry_list.begin (); it != node_entry_list.end (); it++)
    {
      if (it->getDestinationNode () != targetNode.getDestinationNode ())
        {
          double r_new_node = calculateRNew (*it,
                                             targetNode);
          r_new_all = r_new_all + r_new_node;
        }
    }
  weight = r_new_nei_node / r_new_all;
  return weight;
}

/**
 * Method:    calculateRNew
 * Returns:   new Recommendation
 * Parameter: node, targetNode
 */
double IndTrustCal::calculateRNew (TrustTableEntry node,
                                   TrustTableEntry targetNode)
{

  double mlevel = calculateMaturityLevel (node);
  double* rec;
  rec = getDTGT (node,
                 targetNode);
  return mlevel * rec[0];
}

/**
 * Method:    getDTGT
 * Returns:   rec
 * Parameter: node, targetNode
 */
double* IndTrustCal::getDTGT (TrustTableEntry node,
                              TrustTableEntry targetNode)
{

  double* rec;
  rec = sendTRR (node,
                 targetNode);
  return rec;
}

/**
 * Method:    calculateIndirectTrust
 * Returns:   w_sum
 * Parameter: targetNode
 */
double IndTrustCal::calculateIndirectTrust (TrustTableEntry targetNode)
{

  std::vector<TrustTableEntry> node_entry_list = this->trustTable->getTrustTableEntries ();
  double w_sum = 0;

  for (std::vector<TrustTableEntry>::iterator it = node_entry_list.begin (); it != node_entry_list.end (); it++)
    {
      if (it->getDestinationNode () != targetNode.getDestinationNode ())
        {
          double w = calculateWeight (*it,
                                      targetNode);
          double r_new_nei_node = calculateRNew (*it,
                                                 targetNode);
          double *rec;
          rec = getDTGT (*it,
                         targetNode);
          double cal_w_term = w * (r_new_nei_node * rec[1]);

          w_sum = w_sum + cal_w_term;
        }
    }
  double value = round (w_sum * 100000.0) / 100000.0;
  return value;
}

/**
 * Method:    calculateMaturityLevel
 * Returns:   maturity level
 * Parameter: node
 */
double IndTrustCal::calculateMaturityLevel (TrustTableEntry node)
{

  int i_p_node = node.getInteractionCount ();
  std::vector<TrustTableEntry> node_entry_list = this->trustTable->getTrustTableEntries ();
  double i_all = 0;
  for (std::vector<TrustTableEntry>::iterator it = node_entry_list.begin (); it != node_entry_list.end (); it++)
    {
      i_all = i_all + it->getInteractionCount ();
    }
  return i_p_node / i_all;
}


IndTrustCal::~IndTrustCal() {
}

}
}
