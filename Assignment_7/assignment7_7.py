import sys
import scipy
import matplotlib.pyplot as graph
import scipy.special as sp

if len(sys.argv)<2:
    print "No filename passed. Please pass a filename"
elif len(sys.argv)>4:
    print "Too many arguements. Ignoring..."
elif len(sys.argv)==2:
    print "No choice passed. Taking 0 as default"

with file(sys.argv[1],'rb') as txt:
    lines=txt.readlines()
    print lines
    samples=scipy.linspace(0,0,len(lines))
    results=scipy.linspace(0,0,len(lines))
    i=0
    for line in lines:
        samples[i]=line[0]
        results[i]=line[1]
    graph.plot(samples,results)
    graph.show
        
#except:
#    print "%s not found. Plese check if the file exists." %(sys.argv[1])
