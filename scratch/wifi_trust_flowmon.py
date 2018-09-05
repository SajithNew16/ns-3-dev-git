# -*-  Mode: Python; -*-
#  Copyright (c) 2009 INESC Porto
# 
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 2 as
#  published by the Free Software Foundation;
# 
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
# 
#  Authors: Gustavo Carneiro <gjc@inescporto.pt>

import sys
import matplotlib
import os
os.environ['TF_CPP_MIN_LOG_LEVEL']='2'

import pandas as pd
from keras.preprocessing import sequence
from sklearn import preprocessing

matplotlib.use('TkAgg')
import numpy as np
import ns.applications
import ns.core
import ns.flow_monitor
import ns.internet
import ns.mobility
import ns.network
import ns.aodv
import ns.trust
import ns.csma
import ns.wifi
from rlmodel import DQNAgent
from rlmodel import *
from beautifultable import BeautifulTable

try:
    import ns.visualizer
except ImportError:
    pass

DISTANCE = 100 # (m)
NUM_NODES_SIDE = 2
actor = []
action = 0
q_value = 0.0
X_train = []
discount_factor = 0.75

def main(argv, action=None):
     
    cmd = ns.core.CommandLine()

    cmd.NumNodesSide = None
    cmd.AddValue("NumNodesSide", "Grid side number of nodes (total number of nodes will be this number squared)")

    cmd.Results = None
    cmd.AddValue("Results", "Write XML results to file")

    cmd.Plot = None
    cmd.AddValue("Plot", "Plot the results using the matplotlib python module")
	
    cmd.Parse(argv)

    wifi = ns.wifi.WifiHelper.Default()
    wifiMac = ns.wifi.NqosWifiMacHelper.Default()

    wifiPhy = ns.wifi.YansWifiPhyHelper.Default()
    wifiChannel = ns.wifi.YansWifiChannelHelper.Default()
    wifiPhy.SetChannel(wifiChannel.Create())
    ssid = ns.wifi.Ssid("wifi-default")
    wifi.SetRemoteStationManager("ns3::ArfWifiManager")
    wifiMac.SetType ("ns3::AdhocWifiMac",
                     "Ssid", ns.wifi.SsidValue(ssid))

    internet = ns.internet.InternetStackHelper()
    list_routing = ns.internet.Ipv4ListRoutingHelper()
    trust_routing = ns.trust.TrustHelper()
    static_routing = ns.internet.Ipv4StaticRoutingHelper()
    list_routing.Add(static_routing, 0)
    list_routing.Add(trust_routing, 100)
    internet.SetRoutingHelper(list_routing)
    internet.SetRoutingHelper(trust_routing)

    ipv4Addresses = ns.internet.Ipv4AddressHelper()
    ipv4Addresses.SetBase(ns.network.Ipv4Address("10.0.0.0"), ns.network.Ipv4Mask("255.255.255.0"))

    port = 9   # Discard port(RFC 863)
    onOffHelper = ns.applications.OnOffHelper("ns3::UdpSocketFactory",
                                  ns.network.Address(ns.network.InetSocketAddress(ns.network.Ipv4Address("10.0.0.1"), port)))
    onOffHelper.SetAttribute("DataRate", ns.network.DataRateValue(ns.network.DataRate("100kbps")))
    onOffHelper.SetAttribute("OnTime", ns.core.StringValue ("ns3::ConstantRandomVariable[Constant=1]"))
    onOffHelper.SetAttribute("OffTime", ns.core.StringValue ("ns3::ConstantRandomVariable[Constant=0]"))

    addresses = []
    nodes = []

    if action is None:
        action = 0

    if cmd.NumNodesSide is None:
        num_nodes_side = NUM_NODES_SIDE
    else:
        num_nodes_side = int(cmd.NumNodesSide)

    for xi in range(num_nodes_side):
        for yi in range(num_nodes_side):

            node = ns.network.Node()
            nodes.append(node)

            internet.Install(ns.network.NodeContainer(node))

            mobility = ns.mobility.RandomDirection2dMobilityModel()
            mobility.SetPosition(ns.core.Vector(xi*DISTANCE, yi*DISTANCE, 0))
            node.AggregateObject(mobility)
            
            devices = wifi.Install(wifiPhy, wifiMac, node)
            ipv4_interfaces = ipv4Addresses.Assign(devices)
            addresses.append(ipv4_interfaces.GetAddress(0))

    for i, node in enumerate(nodes):
        destaddr = addresses[action]
        onOffHelper.SetAttribute("Remote", ns.network.AddressValue(ns.network.InetSocketAddress(destaddr, port)))
        app = onOffHelper.Install(ns.network.NodeContainer(node))
        urv = ns.core.UniformRandomVariable()
        app.Start(ns.core.Seconds(urv.GetValue(20, 30)))   
	
    flowmon_helper = ns.flow_monitor.FlowMonitorHelper()
    monitor = flowmon_helper.InstallAll()
    monitor = flowmon_helper.GetMonitor()
    monitor.SetAttribute("DelayBinWidth", ns.core.DoubleValue(0.001))
    monitor.SetAttribute("JitterBinWidth", ns.core.DoubleValue(0.001))
    monitor.SetAttribute("PacketSizeBinWidth", ns.core.DoubleValue(20))

    routing_stream = ns.network.OutputStreamWrapper("routes", ns.network.STD_IOS_OUT) 
    trust_routing.PrintRoutingTableAllAt (ns.core.Seconds(25), routing_stream)

    ns.core.Simulator.Stop(ns.core.Seconds(100.0))
    ns.core.Simulator.Run()

    monitor.CheckForLostPackets()
    classifier = flowmon_helper.GetClassifier() 

    if cmd.Results is None:       
       X_prev = []
       X_train = []
       reward =0 
       actor = DQNAgent(3, num_nodes_side)
       for e in range(2):
        if e > 0:
            for i in range(1,((num_nodes_side**(2))+1)):
		for flow_id, flow_stats in monitor.GetFlowStats():
            		t = classifier.FindFlow(flow_id)
            		proto = {6: 'TCP', 17: 'UDP'} [t.protocol]
			if(t.sourceAddress == (ns.network.Ipv4Address("10.0.0."+str(i)))):
				flow_array = []
				flow_array.append(flow_stats.txBytes)
				flow_array.append(flow_stats.rxBytes)
				flow_array.append(flow_stats.txPackets)
				flow_array.append(flow_stats.rxPackets)
				flow_array.append(flow_stats.lostPackets)
					    
				if flow_stats.rxPackets > 0:
					mean_delay = (flow_stats.delaySum.GetSeconds() / flow_stats.rxPackets)
					flow_array.append(mean_delay)
				if flow_stats.rxPackets - 1 > 0:
					mean_jitter = (flow_stats.jitterSum.GetSeconds() / (flow_stats.rxPackets - 1))
					flow_array.append(mean_jitter)
				else:
					flow_array.append(0)                         
				data = np.array(flow_array)
				df = pd.DataFrame(data)
				min_max_scaler = preprocessing.MinMaxScaler() 
				np_scaled = min_max_scaler.fit_transform(df)
								
				if flow_id != 1:
					X_prev = X_train
				
				X_train = sequence.pad_sequences(np_scaled, maxlen=7, dtype='double', padding='pre', truncating='pre',
								             value=0.) 
				action = actor.act(X_train) 
								 
				try:
					reward=((0.1*(flow_stats.delaySum.GetSeconds() / flow_stats.rxPackets)+0.2*(flow_stats.jitterSum.GetSeconds() / (flow_stats.rxPackets-1))+0.1*(float(flow_stats.timesForwarded) / flow_stats.rxPackets + 1)+(0.5*(flow_stats.rxPackets)/(flow_stats.txPackets))) + 0.1*(flow_stats.lostPackets))

				except ZeroDivisionError:
					flow_stats.delaySum.GetSeconds() == 0
					flow_stats.jitterSum.GetSeconds() == 0
					    
			 	q_value = actor.q_values(X_train)		
					    
				if flow_id != 1:
					actor.remember(X_prev, action,reward, X_train,False)            
													  
				table = BeautifulTable()
				table.column_headers = ["Flow id","Source", "Destination", "reward","q_value"]
				table.append_row([flow_id, t.sourceAddress,t.destinationAddress, reward,q_value])
				print(table)
					
		actor.remember(X_prev, action, reward, X_train, True)

    else:
        print monitor.SerializeToXmlFile(cmd.Results, True, True)

      
    if cmd.Plot is not None:
        import pylab
        delays = []
        for flow_id, flow_stats in monitor.GetFlowStats():
            tupl = classifier.FindFlow(flow_id)
            if tupl.protocol == 17 and tupl.sourcePort == 698:
                continue
            try:
               delays.append(flow_stats.delaySum.GetSeconds() / flow_stats.rxPackets)
            except ZeroDivisionError:
               flow_stats.delaySum.GetSeconds() == 0
            
        pylab.hist(delays, 20)
        pylab.xlabel("Delay (s)")
        pylab.ylabel("Number of Flows")
        pylab.show() 

    if cmd.Plot is not None:
        import pylab
        jitters = []
        for flow_id, flow_stats in monitor.GetFlowStats():
            tupl = classifier.FindFlow(flow_id)
            if tupl.protocol == 17 and tupl.sourcePort == 698:
                continue
   	    try:
               jitters.append(flow_stats.jitterSum.GetSeconds() / flow_stats.rxPackets)
	    except ZeroDivisionError:            
		flow_stats.jitterSum.GetSeconds() == 0

        pylab.hist(jitters, 20)
        pylab.xlabel("Jitter (s)")
        pylab.ylabel("Number of Flows")
        pylab.show()


    if cmd.Plot is not None:
        import pylab
        hopCounts = []
        for flow_id, flow_stats in monitor.GetFlowStats():
            tupl = classifier.FindFlow(flow_id)
            if tupl.protocol == 17 and tupl.sourcePort == 698:
                continue
            try:
               hopCounts.append(flow_stats.timesForwarded/ flow_stats.rxPackets+1)
            except ZeroDivisionError:            
		flow_stats.jitterSum.GetSeconds() == 0
        
	pylab.hist(hopCounts, 20)
        pylab.xlabel("HopCount (s)")
        pylab.ylabel("Number of Flows")
        pylab.show()

  

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))

