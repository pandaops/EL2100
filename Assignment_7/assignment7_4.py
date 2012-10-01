import scipy
import matplotlib.pyplot as graph
import math
import scipy.special as sp

samples=scipy.linspace(-3,+3,101)
results=scipy.linspace(0,0,101)
for x in samples:
    index=scipy.nonzero(samples==x)[0][0]
    for k in range(1,11,2):
        results[index]+= (sp.sin(k*x)/k)

graph.plot(samples,results,'ro',samples,results,'k')
graph.show()


