import scipy
import matplotlib.pyplot as graph
import math
import scipy.special as sp

samples=scipy.linspace(-3,+3,101)
results=scipy.linspace(0,0,101)

# Vector operation for each k
for k in range(1,11,2):
    results += sp.sin(k*samples)/k

# Graph plotting
graph.plot(samples,results,'ro',samples,results,'k')
graph.show()


