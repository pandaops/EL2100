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
L = loadtxt('fitting.dat',unpack=True)

# 4. Declare the function g and compute
def g(t,a,b):
        g = a*sp.jn(2,t) + b*t 
        return g;

real_values = g(L[0],A,B).reshape(101,-1)
        
# 3. Plotting with varying levels of noise
graph.figure(1)
graph.xlabel('$t$')
graph.ylabel('$F_\sigma$')
for i in range(1,9,2):
    graph.plot(L[0],L[i],'+')
    graph.plot(L[0],L[i+1],'.')
graph.plot(L[0],real_values,'k')
graph.legend(('$log(\sigma) = -1$','$log(\sigma) = -0.5$','$log(\sigma) = 0$',
'$log(\sigma) = 0.5$','$log(\sigma) = 1$','$log(\sigma) = 1.5$','$log(\sigma) = 2$',
'$log(\sigma) = 2.5$','$log(\sigma) = 3$','Actual function'),loc=0)

# 5. Compute G with the matrix method
j = sp.jn(2,L[0])
M = c_[j,L[0]]
AB = [[A],[B]]
g_alt = dot(M,AB)

if all(g_alt)==all(real_values):
    print "Matrix and Algebraic methods both give equal values"

AA=arange(0,2.1,0.1)
BB=arange(-.05,0.0025,0.0025)
epsilon = np.zeros((len(AA),len(BB)))
sum1 = 0

# 6. Computing the mean squared error
for a in range(len(AA)):
        for b in range(len(BB)):
                epsilon[a][b]=sum((L[1] - g(L[0],AA[a],BB[b]))**2)
epsilon/=101

# 7. Plotting the Countour
graph.figure(2)
graph.contour(BB,AA,epsilon)
graph.xlabel('$A$')
graph.ylabel('$B$')
graph.title('Contour plot of A and B')
p=[]
errors =[]

# 8., 9. Estimating error
for x in range(1,10):
    least=lstsq(M,L[x].reshape(101,-1))[0]
    p.append(least)
    err = sqrt(dot(transpose(dot(M,least) - L[x].reshape(101,-1)),dot(M,least) - L[x].reshape(101,-1)))        
    errors=append(errors,err[0][0])
p=array(p)
sigma = logspace(-1,-3,9)

# 9. Plotting normal error 
graph.figure(3)
A = np.vstack([sigma, np.ones(len(sigma))]).T
m, c = np.linalg.lstsq(A, errors)[0]
graph.plot(sigma, m*sigma + c, 'k', label='Fitted line')
graph.plot(sigma,errors,'ro')
graph.xlabel(r"noise $\sigma$")
graph.ylabel('error')
graph.title('Plot of error in estimate of A and B vs noise')

# 10. Logarithmic error
graph.figure(4)
graph.loglog(sigma,errors,'k')
graph.loglog(sigma,errors,'ro')
graph.xlabel(r"log($\sigma$)")
graph.ylabel('log(error)')
graph.title('Plot of logarithmic error in estimate of A and B vs noise')

# 11. Finding the exponents
noise=log(sigma).reshape((len(log(sigma)),1))
sigma=hstack((ones((len(log(sigma)),1)),noise))
solution=lstsq(sigma,log(errors))
print 'Alpha=%f'%pow(e,solution[0][0])
print 'Beta=%f'%solution[0][1]

graph.show()
