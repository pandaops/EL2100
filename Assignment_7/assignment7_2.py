import scipy
import matplotlib.pyplot as graph
import math
import scipy.special as step

# Parameters
MAX_ITER=1000
MIN_ERR=1e-4

samples = step.arange(1,100,0.1)
results = scipy.linspace(0,0,len(samples))

# Calculate series till least error is achieved
for x in samples:
    index=scipy.nonzero(samples==x)[0][0]
    sign=1   
    for n in range(1,MAX_ITER):
       t = sign*x/(x*x+n*n)
       results[index] += t
       sign = -sign
       if(abs(t)<MIN_ERR):
           break

# Write to file
with file('output.dat','w') as output:            
    for xx in scipy.c_[samples,results]:
        output.write(str(xx[0])+'\t'+str(xx[1])+'\n')


