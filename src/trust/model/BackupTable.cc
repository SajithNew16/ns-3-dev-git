#include "BackupTable.h"
#include <iostream>

namespace ns3
{
namespace trust
{


BackupTable::BackupTable()
{
	columnSeperator = " | ";
}


void BackupTable::addBackupTableEntry(BackupTableEntry entry)
{
	backupTableRecords.push_back(entry);
}

std::vector<BackupTableEntry>& BackupTable::getBackupTableEntries()
{
	return backupTableRecords;
}

void
BackupTable::printTable()
{
	uint n = 5;
	if (backupTableRecords.size() >= n)
	  {
	    std::vector<BackupTableEntry> y(backupTableRecords.end() - n, backupTableRecords.end());
	    std::cout << ">>>>>>>>>>>>>> Last 5 records of Backup Table <<<<<<<<<<<<<<<" << std::endl;
	    std::cout << "| " << "Node" << columnSeperator << "Trust Value" << columnSeperator << "Time Duration" << columnSeperator << "Analyzed Result" << std::endl;
	    for (std::vector<BackupTableEntry>::iterator it = y.begin(); it != y.end(); it++)
	      {
            std::cout << "| " << it->getNeiNode() << columnSeperator << it->getTrustValue() << "\t" << columnSeperator << it->GetTimeDuration () << "\t\t" << columnSeperator << it->GetResult () << std::endl;
          }
	  }
	else
	  {
		std::cout << ">>>>>>>>>>>>>> Backup Table <<<<<<<<<<<<<<<" << std::endl;
		std::cout << "| " << "Node" << columnSeperator << "Trust Value" << columnSeperator << "Time Duration" << columnSeperator << "Analyzed Result" << std::endl;
		for (std::vector<BackupTableEntry>::iterator it = backupTableRecords.begin(); it != backupTableRecords.end(); it++)
		  {
		    std::cout << "| " << it->getNeiNode() << columnSeperator << it->getTrustValue() << "\t" << columnSeperator << it->GetTimeDuration () << "\t\t" << columnSeperator << it->GetResult () << std::endl;
		  }
	  }
}

void BackupTable::addToTrustList(double trustValue)
{
	this->trustList.push_back(trustValue);
}

std::vector<double>& BackupTable::getTrustList()
{
	return this->trustList;
}


BackupTable::~BackupTable()
{
}

}
}
