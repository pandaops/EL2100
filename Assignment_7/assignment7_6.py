#! /usr/bin/env python

# The above shebang allows for *nix systems to
# automatically open the file.

import sys
import scipy
import matplotlib.pyplot as graph
import scipy.special as sp

if len(sys.argv)<2:
    print "No filename passed. Please pass a filename"
    exit()
elif len(sys.argv)>4:
    print "Too many arguements. Ignoring..."
elif len(sys.argv)==2:
    print "No choice passed. Taking 0 as default"

samples,results =scipy.loadtxt(sys.argv[1],usecols=(0,1),unpack=True)
try:
    choice = sys.argv[2]
except:
    choice = '0'

if (choice=='0'):
    graph.plot(samples,results)
elif (choice=='1'):
    graph.semilogy(samples,results)
elif (choice=='2'):
    graph.loglog(samples,results)
else:
    print "Invalid Choice"
    exit

graph.show()

