import scipy
import matplotlib.pyplot as graph
import math
import scipy.special as sp

samples=scipy.linspace(0,5,num=101)
results=scipy.linspace(0,0,num=101)

for x in samples:
    results[scipy.nonzero(samples==x)[0][0]]=sp.sin(x*x)
    
graph.plot(samples,results,'ro',samples,results,'k')
graph.show()
