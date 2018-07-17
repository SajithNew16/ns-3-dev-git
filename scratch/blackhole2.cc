/* Blackhole Attack Simulation with AODV Routing Protocol - Sample Program
 * 
 * Network topology
 * 
 *    n0 ------------> n1 ------------> n2 -------------> n3
 * 
 * Each node is in the range of its immediate adjacent.
 * Source Node: n1
 * Destination Node: n3
 * Malicious Node: n0
 * 
 * Output of this file:
 * 1. Generates blackhole.routes file for routing table information and
 * 2. blackhole.xml file for viewing animation in NetAnim.
 * 
 */
#include "ns3/aodv-module.h"
#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/netanim-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/mobility-module.h"
#include "myapp.h"

NS_LOG_COMPONENT_DEFINE ("Blackhole2");

using namespace ns3;



void
ReceivePacket(Ptr<const Packet> p, const Address & addr)
{
	std::cout << Simulator::Now ().GetSeconds () << "\t" << p->GetSize() <<"\n";
}


int main (int argc, char *argv[])
{
  bool enableFlowMonitor = false;
  std::string phyMode ("DsssRate1Mbps");

  CommandLine cmd;
  cmd.AddValue ("EnableMonitor", "Enable Flow Monitor", enableFlowMonitor);
  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.Parse (argc, argv);

//
// Explicitly create the nodes required by the topology (shown above).
//
  NS_LOG_INFO ("Create nodes.");
  NodeContainer c; // ALL Nodes
  NodeContainer not_malicious;
  NodeContainer malicious;
  c.Create(20);

  not_malicious.Add(c.Get(1));
  not_malicious.Add(c.Get(0));
  not_malicious.Add(c.Get(3));
  not_malicious.Add(c.Get(5));
  not_malicious.Add(c.Get(6));
  not_malicious.Add(c.Get(7));
  not_malicious.Add(c.Get(8));
  not_malicious.Add(c.Get(9));
  not_malicious.Add(c.Get(10));
  not_malicious.Add(c.Get(11));
  not_malicious.Add(c.Get(12));
  not_malicious.Add(c.Get(13));
  not_malicious.Add(c.Get(16));
  not_malicious.Add(c.Get(17));
  not_malicious.Add(c.Get(18));
  not_malicious.Add(c.Get(19));
  not_malicious.Add(c.Get(14));
  not_malicious.Add(c.Get(15));
  malicious.Add(c.Get(4));
  malicious.Add(c.Get(2));

  // Set up WiFi
  WifiHelper wifi;

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);

  YansWifiChannelHelper wifiChannel ;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::TwoRayGroundPropagationLossModel",
	  	  	  	  	  	  	  	    "SystemLoss", DoubleValue(1),
		  	  	  	  	  	  	    "HeightAboveZ", DoubleValue(1.5));

  // For range near 250m
  wifiPhy.Set ("TxPowerStart", DoubleValue(33));
  wifiPhy.Set ("TxPowerEnd", DoubleValue(33));
  wifiPhy.Set ("TxPowerLevels", UintegerValue(1));
  wifiPhy.Set ("TxGain", DoubleValue(0));
  wifiPhy.Set ("RxGain", DoubleValue(0));
  wifiPhy.Set ("EnergyDetectionThreshold", DoubleValue(-61.8));
  wifiPhy.Set ("CcaMode1Threshold", DoubleValue(-64.8));

  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add a non-QoS upper mac
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifiMac.SetType ("ns3::AdhocWifiMac");

  // Set 802.11b standard
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);

  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue(phyMode),
                                "ControlMode",StringValue(phyMode));


  NetDeviceContainer devices;
  devices = wifi.Install (wifiPhy, wifiMac, c);


//  Enable AODV
  AodvHelper aodv;
  AodvHelper malicious_aodv; 
 

  // Set up internet stack
  InternetStackHelper internet;
  internet.SetRoutingHelper (aodv);
  internet.Install (not_malicious);
  
  malicious_aodv.Set("IsMalicious",BooleanValue(true)); // putting *false* instead of *true* would disable the malicious behavior of the node
  internet.SetRoutingHelper (malicious_aodv);
  internet.Install (malicious);

  // Set up Addresses
  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer ifcont = ipv4.Assign (devices);


  NS_LOG_INFO ("Create Applications.");

  // UDP connection from N1 to N3

  uint16_t sinkPort = 6;
  Address sinkAddress (InetSocketAddress (ifcont.GetAddress (3), sinkPort)); // interface of n3
  PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
  ApplicationContainer sinkApps = packetSinkHelper.Install (c.Get (3)); //n3 as sink
  sinkApps.Start (Seconds (0.));
  sinkApps.Stop (Seconds (100.));

  Ptr<Socket> ns3UdpSocket = Socket::CreateSocket (c.Get (1), UdpSocketFactory::GetTypeId ()); //source at n1

  // Create UDP application at n1
  Ptr<MyApp> app = CreateObject<MyApp> ();
  app->Setup (ns3UdpSocket, sinkAddress, 1040, 5, DataRate ("250Kbps"));
  c.Get (1)->AddApplication (app);
  app->SetStartTime (Seconds (4.));
  app->SetStopTime (Seconds (100.));

// Set Mobility for all nodes

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject <ListPositionAllocator>();
  positionAlloc ->Add(Vector(100, 0, 0)); // node0
  positionAlloc ->Add(Vector(200, 0, 0)); // node1 
  positionAlloc ->Add(Vector(450, 0, 0)); // node2
  positionAlloc ->Add(Vector(550, 0, 0)); // node3
  positionAlloc ->Add(Vector(650, 0, 0)); // node4
  positionAlloc ->Add(Vector(750, 0, 0)); // node5
  positionAlloc ->Add(Vector(850, 0, 0)); // node6
  positionAlloc ->Add(Vector(950, 0, 0)); // node7
  positionAlloc ->Add(Vector(1000, 0, 0)); // node8
  positionAlloc ->Add(Vector(250, 0, 0)); // node9
  positionAlloc ->Add(Vector(800, 0, 0)); // node10
  positionAlloc ->Add(Vector(900, 0, 0)); // node11
  positionAlloc ->Add(Vector(300, 0, 0)); // node13
  positionAlloc ->Add(Vector(400, 0, 0)); // node14
  positionAlloc ->Add(Vector(500, 0, 0)); // node15
  positionAlloc ->Add(Vector(600, 0, 0)); // node16
  positionAlloc ->Add(Vector(700, 0, 0)); // node17
  positionAlloc ->Add(Vector(350, 0, 0)); // node18
  positionAlloc ->Add(Vector(975, 0, 0)); // node19

  mobility.SetPositionAllocator(positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  /*mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue (0.0),
                                   "MinY", DoubleValue (0.0),
                                   "DeltaX", DoubleValue (100),
                                   "DeltaY", DoubleValue (0),
                                   "GridWidth", UintegerValue (4),
                                   "LayoutType", StringValue ("RowFirst"));
  mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel");*/
  mobility.Install(c);


  AnimationInterface anim ("blackhole2.xml"); // Mandatory
  AnimationInterface::SetConstantPosition (c.Get (0), 0, 400);
  AnimationInterface::SetConstantPosition (c.Get (1), 200, 300);
  AnimationInterface::SetConstantPosition (c.Get (2), 200, 500);
  AnimationInterface::SetConstantPosition (c.Get (3), 500, 400);
  AnimationInterface::SetConstantPosition (c.Get (4), 400, 350);
  AnimationInterface::SetConstantPosition (c.Get (5), 450, 250);
  AnimationInterface::SetConstantPosition (c.Get (6), 500, 150);
  AnimationInterface::SetConstantPosition (c.Get (7), 700, 100);
  AnimationInterface::SetConstantPosition (c.Get (8), 900, 200);
  AnimationInterface::SetConstantPosition (c.Get (9), 900, 250);
  AnimationInterface::SetConstantPosition (c.Get (10), 1000, 350);
  AnimationInterface::SetConstantPosition (c.Get (11), 700, 250);
  AnimationInterface::SetConstantPosition (c.Get (12), 700, 350);
  AnimationInterface::SetConstantPosition (c.Get (13), 700, 500);
  AnimationInterface::SetConstantPosition (c.Get (14), 100, 250);
  AnimationInterface::SetConstantPosition (c.Get (15), 300, 250);
  AnimationInterface::SetConstantPosition (c.Get (16), 300, 100);
  AnimationInterface::SetConstantPosition (c.Get (17), 200, 100);
  AnimationInterface::SetConstantPosition (c.Get (18), 900, 500);
  AnimationInterface::SetConstantPosition (c.Get (19), 900, 350);
  anim.UpdateNodeSize (0, 10, 10);
  anim.UpdateNodeSize (1, 10, 10);
  anim.UpdateNodeSize (2, 10, 10);
  anim.UpdateNodeSize (3, 10, 10);
  anim.UpdateNodeSize (4, 10, 10);
  anim.UpdateNodeSize (5, 10, 10);
  anim.UpdateNodeSize (6, 10, 10);
  anim.UpdateNodeSize (7, 10, 10);
  anim.UpdateNodeSize (8, 10, 10);
  anim.UpdateNodeSize (9, 10, 10);
  anim.UpdateNodeSize (10, 10, 10);
  anim.UpdateNodeSize (11, 10, 10);
  anim.UpdateNodeSize (12, 10, 10);
  anim.UpdateNodeSize (13, 10, 10);
  anim.UpdateNodeSize (14, 10, 10);
  anim.UpdateNodeSize (15, 10, 10);
  anim.UpdateNodeSize (16, 10, 10);
  anim.UpdateNodeSize (17, 10, 10);
  anim.UpdateNodeSize (18, 10, 10);
  anim.UpdateNodeSize (19, 10, 10);
  anim.UpdateNodeColor (2,0,0,0);
  anim.UpdateNodeColor (4,0,0,0);
  anim.EnablePacketMetadata(true);

  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("blackhole2.routes", std::ios::out);
  aodv.PrintRoutingTableAllAt (Seconds (5.5), routingStream);

  // Trace Received Packets
  Config::ConnectWithoutContext("/NodeList/*/ApplicationList/*/$ns3::PacketSink/Rx", MakeCallback (&ReceivePacket));


//
// Calculate Throughput using Flowmonitor
//
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();


//
// Now, do the actual simulation.
//
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds(100.0));
  Simulator::Run ();

  monitor->CheckForLostPackets ();

  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
	  Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      if ((t.sourceAddress=="10.0.0.4" && t.destinationAddress == "10.0.0.6"))
      {
          std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
          std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
          std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
          std::cout << "  Delivery %:   " << (float)i->second.rxBytes / (float)i->second.txBytes * 100 << "%\n";
      	  std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps\n";
      }
     }

  monitor->SerializeToXmlFile("aodvflow2.flowmon", true, true);
//Flow 1 (10.1.2.2 -> 10.1.2.4)

}
