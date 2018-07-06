#pragma once
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include <string>

namespace ns3
{
namespace trust
{

class BackupTableEntry
{
private:
	Ipv4Address neiNode;
	double trustValue;
	Time timeDuration;
	std::string result;
public:
	BackupTableEntry ();
	void SetNeiNode (Ipv4Address neiNode);
	Ipv4Address GetNeiNode ();
	void SetTrustValue (double trustValue);
	double GetTrustValue ();
	void SetResult (std::string result);
	std::string GetResult ();
	void SetTimeDuration (Time timeDuration);
	Time GetTimeDuration ();
	~BackupTableEntry ();
};

}
}
