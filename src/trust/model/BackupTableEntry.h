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
	BackupTableEntry();
	void setNeiNode(Ipv4Address neiNode);
	Ipv4Address getNeiNode();
	void setTrustValue(double trustValue);
	double getTrustValue();
	void updateTrustValue(double trustValue);
	void SetResult (std::string result);
	void updateResult(std::string result);
	std::string GetResult ();
	void SetTimeDuration (Time timeDuration);
	Time GetTimeDuration ();
	void updateTimeDuration(Time timeDuration);
	~BackupTableEntry();
};

}
}
