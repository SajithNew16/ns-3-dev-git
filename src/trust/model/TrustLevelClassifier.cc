#include "TrustLevelClassifier.h"
#include "RecommendationTableEntry.h"
#include "IndTrustCal.h"
#include "TrustTableEntry.h"
#include "DirTrustCal.h"
#include "Spiral.h"
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
				it->setBlacklist(false);
			}
			else
			{
				it->setTrustLevel(2);
				it->setBlacklist(false);
			}
		}
		else
		{
			if (node_GT > Threshold_selfish)
			{
				it->setTrustLevel(3);
				it->setBlacklist(false);
				calculateReductionFactor(*it);
				it->calculateGlobalTrust ();
			}
			else
            {
              //----call spiral model here-----
              Spiral spiralModel;
              double *past_global_trust_range;
              past_global_trust_range = spiralModel.GetMinMaxTrust (this->backupTable,
                                                                    *it);
              if (past_global_trust_range[0] <= node_GT && past_global_trust_range[1] >= node_GT)
                {
                  it->setTrustLevel (4);
                  it->setBlacklist (false);
                }
              else
                {
                  it->setTrustLevel (5);
                  it->setBlacklist (true);
                  calculateReductionFactorForNeibors (trustTable,
                                                      *it);
                }
            }
		 }
	}
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

void TrustLevelClassifier::calculateReductionFactor(TrustTableEntry trustTableEntry)
{
	double DT,GT,IT;
	double reductionVal = 0;

	DT = trustTableEntry.getDirectTrust();
	GT = trustTableEntry.getGlobalTrust();
	IT = trustTableEntry.getIndirectTrust();
	reductionVal = DT/(GT*10);

//	std::cout<<"IT val Before------>>>>"<< trustTableEntry.getIndirectTrust()<<std::endl;
	trustTableEntry.setIndirectTrust(IT - reductionVal);
//	std::cout<<"IT val After------>>>>"<< trustTableEntry.getIndirectTrust()<<std::endl;

}

void
TrustLevelClassifier::calculateReductionFactorForNeibors(TrustTable* trustTable,TrustTableEntry trustTableEntry)
{
  double reductionVal = 0.0;
  for (std::vector<TrustTableEntry>::iterator it = trustTable->getTrustTableEntries ().begin ();
      it != trustTable->getTrustTableEntries ().end (); it++)
    {
      //select neighbor nodes
      if (trustTableEntry.getDestinationNode () != it->getDestinationNode ())
        {
          //update global trust for neighbor nodes
          reductionVal = it->getDirectTrust()/(it->getGlobalTrust()*10);
          it->updateIndirectTrust(it->getIndirectTrust() - reductionVal);
          it->calculateGlobalTrust();
        }
    }
}

void
TrustLevelClassifier::SetBackupTable (BackupTable* backupTable)
{
	this->backupTable = backupTable;
}


TrustLevelClassifier::~TrustLevelClassifier()
{
}

}
}
