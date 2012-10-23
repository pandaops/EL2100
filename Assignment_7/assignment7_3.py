import scipy
import matplotlib.pyplot as graph
import math
import scipy.special as sp

samples=scipy.linspace(0,5,num=101)
results=scipy.linspace(0,0,num=101)

# Vector assignment
results = sp.sin(samples*samples)

# Plotting the graph
graph.plot(samples,results,'ro',samples,results,'k')
graph.show()
