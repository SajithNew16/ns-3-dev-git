#include "BackupTableEntry.h"

namespace ns3
{
namespace trust
{

BackupTableEntry::BackupTableEntry()
{
}

void
BackupTableEntry::SetNeiNode (Ipv4Address neiNode)
{
	this->neiNode = neiNode;
}

Ipv4Address
BackupTableEntry::GetNeiNode ()
{
	return this->neiNode;
}

void
BackupTableEntry::SetTrustValue (double trustValue)
{
	this->trustValue = trustValue;
}

double
BackupTableEntry::GetTrustValue ()
{
	return this->trustValue;
}

void
BackupTableEntry::SetResult (std::string result)
{
	this->result = result;
}

std::string
BackupTableEntry::GetResult()
{
	return this->result;
}

void
BackupTableEntry::SetTimeDuration (Time timeDuration)
{
	this->timeDuration = timeDuration;
}

Time
BackupTableEntry::GetTimeDuration()
{
	return this->timeDuration;
}

BackupTableEntry::~BackupTableEntry()
{
}

}
}
