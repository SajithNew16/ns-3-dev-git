import sys
import matplotlib
import os
os.environ['TF_CPP_MIN_LOG_LEVEL']='2'
import tensorflow as tf

import pandas as pd
from keras.preprocessing import sequence
from sklearn import preprocessing

matplotlib.use('TkAgg')
import numpy as np
import scipy
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

DISTANCE = 250 # (m)
NUM_NODES_SIDE = 5
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

    wifi = ns.wifi.WifiHelper
    wifiMac = ns.wifi.NqosWifiMacHelper.Default()
    wifiPhy = ns.wifi.YansWifiPhyHelper.Default()
    wifiChannel = ns.wifi.YansWifiChannelHelper.Default()

    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))

