import scipy
import matplotlib.pyplot as graph
import math

samples=scipy.linspace(-math.pi,math.pi,num=201)

def fourier_series(x):
    return math.sin(x) + math.sin(3*x)/3 + math.sin(5*x)/5 + math.sin(7*x)/7

results = map(fourier_series,samples)

for xx in scipy.c_[samples,results]:
    print "%.4f: %.4f" % (xx[0],xx[1])

graph.plot(samples,results,'ro')
graph.show()
