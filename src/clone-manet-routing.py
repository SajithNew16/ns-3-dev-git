#! /usr/bin/env python
#   clone-manet-routing.py - Python Script to create a clone of AODV / DSDV / OLSR / DSR in ns-3
#   Copyright (C) 2015 Wireless Information Networking Group (WiNG), NITK Surathkal, Mangalore, India.
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License version 3 as published by
#   the Free Software Foundation.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   /************************************************************************/
#   /*                                                                      */
#   /*      Designed and Developed by :                                     */
#   /*      Kriti Nagori, Meenakshy Balachandran and Mohit P Tahiliani      */
#   /*      Wireless Information Networking Group(WiNG)                     */ 
#   /*      National Institute of Technology Karnataka, Surathkal           */
#   /*      Mangalore, Karnataka, INDIA.                                    */
#   /*      wing@nitk.ac.in , nitkwing@gmail.com                            */ 
#   /*      http://wing.nitk.ac.in                                          */
#   /*                                                                      */
#   /************************************************************************/

import sys
from optparse import OptionParser, SUPPRESS_HELP
import os
import shutil, errno
import fileinput

def main(argv):
    
    parser = OptionParser(usage=("Usage: %prog sourcemodule destinationmodule\n"
                                 "Utility script to clone an existing module in ns-3"))
    (options, args) = parser.parse_args()

    # Checking the number of command line arguments
    if len(args) != 2:					
        print "Enter exactly 2 command line arguments"
        parser.print_help()
        return 1
    
    # Validation of argument 1 syntax
    smodname = args[0].lower()
    if False in [word.isalnum() for word in smodname]:
        print >> sys.stderr, "Module name should only contain alphanumeric characters"
        return 2
    assert os.path.sep not in smodname
    
    smoduledir = os.path.join(os.path.dirname(__file__), smodname)
    if not (os.path.exists(smoduledir)):
        print >> sys.stderr, "Module %r doesnt exist" % (smodname,)
        return 2
    if smodname not in ['aodv','dsdv','olsr', 'dsr']:
        print >> sys.stderr, "Only AODV / DSDV / OLSR / DSR can be cloned"
        return 2
    
    # Validation of argument 2 syntax
    dmodname = args[1].lower()
    if False in [word.isalnum() for word in dmodname]:
        print >> sys.stderr, "Module name should only contain alphanumeric characters"
        return 2
    assert os.path.sep not in dmodname
    
    dmoduledir = os.path.join(os.path.dirname(__file__), dmodname)
    if os.path.exists(dmoduledir):
        print >> sys.stderr, "Module %r already exists" % (dmodname,)
        return 2
    print("Cloning module %r, "
          "run './waf configure' and './waf' to include it in the build" % (dmodname,)) 
    try:
        shutil.copytree(smoduledir, dmoduledir)
    
    # Directories are the same
    except shutil.Error as e:
        print('Directory not copied. Error: %s' % e)
    
    # Any error saying that the directory doesn't exist
    except OSError as e:
        print('Directory not copied. Error: %s' % e)   
    
    # Storing the names to replace in all 3 cases
    orgupper = smodname.upper()
    orglower = smodname.lower()
    orgtitle = smodname.title()
    newupper = dmodname.upper()
    newlower = dmodname.lower()
    newtitle = dmodname.title() 
    
    # Renaming and Replacing loop
    for root, dirs, files in os.walk(dmoduledir, topdown=False):
        for name in files:
            newname = name.replace(smodname,dmodname)
            os.rename(os.path.join(root, name),os.path.join(root, newname))
            files = fileinput.FileInput(os.path.join(root, newname), inplace=True)
            for line in files:
            	if orgupper in line:
                    line = line.replace(orgupper, newupper)
                if orglower in line:    
                    line = line.replace(orglower, newlower)
                if orgtitle in line:    
                    line = line.replace(orgtitle, newtitle)
                print(line), 
    return 0
if __name__ == '__main__':
    sys.exit(main(sys.argv))
