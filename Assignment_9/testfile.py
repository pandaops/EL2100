#! usr/bin/python

import sys
from scipy import *
import matplotlib.pyplot as graph
import scipy.special as sp
from scipy.linalg import lstsq
import numpy as np


A = 1
B = -0.02546303

# 2. loading file and reshaping
L = loadtxt('fitting.dat')
t = L[:,0]
values = L[:,1:] 

# 4. Declare the function g and compute
def g(t,a,b):
        g = a*sp.jn(2,t) + b*t 
        return g;

real_values = g(t,A,B)
        
# 3. Plotting with varying levels of noise
graph.figure(1)
graph.xlabel('$t$')
graph.ylabel('$F_\sigma$')
graph.plot(t,values[:,:5],'+')
graph.plot(t,values[:,5:],'.')
graph.plot(t,real_values,'k')
graph.legend(('$log(\sigma) = -1$','$log(\sigma) = -0.5$','$log(\sigma) = 0$',
'$log(\sigma) = 0.5$','$log(\sigma) = 1$','$log(\sigma) = 1.5$','$log(\sigma) = 2$',
'$log(\sigma) = 2.5$','$log(\sigma) = 3$','Actual function'),loc=0)

# 5. Compute G with the matrix method
j = sp.jn(2,t)
M = c_[j,t]
AB = [[A],[B]]
g_alt = dot(M,AB)

sample = np.asarray([val[0] for val in values])
AA = sp.linspace(0,2,21)
BB = sp.linspace(-0.05, 0, 21)
epsilon = np.zeros((len(AA),len(BB)))
f1 = L[:,1].reshape(101,1)

# 6. Computing the mean squared error
for a in range(len(AA)):
        for b in range(len(BB)):
                epsilon[a,b]=sum((sample - g(t,AA[a],BB[b]))**2)
                
graph.figure(2)
graph.contour(AA,BB,epsilon)
graph.ylim(-0.05,0)
graph.xlabel('$A$')
graph.ylabel('$B$')
graph.title('Contour plot of A and B')


graph.show()

