from scipy.integrate import quad 
from scipy import linspace, cumsum, where
import matplotlib.pyplot as graph
import scipy.special as sp
from numpy import fabs

# The function that Quad uses
func=lambda x: 1/(1+x**2)
step=0.1
err=1
counter=0
steps = []

samples=sp.arange(0,5+step,step)
results = sp.linspace(0,0,len(samples))
manual= sp.linspace(0,0,len(samples))
estimated_errors, real_errors=[],[]

def integrate(samples,results):
	for i in range(len(samples)):
		# Compute Results with Quad
		results[i]=quad(func,0,samples[i])[0]
	return results

def series(new_samples,new_results):
	# Compute manually using recursive summation
    for i in range(len(new_samples)):	
        new_results[i]=step*(cumsum(func(new_samples))[i] + 0.5*(func(new_samples[0])+func(new_samples[i])))
    return new_results

def ftrap(oldx,oldy):
    newx = sp.arange(oldx[0],oldx[-1]+(0.5)*step,0.5*step)
    newy = sp.linspace(0,0,len(newx))
    return (newx,newy)
    
def error(old,new):
    if new is None:
        return 1
    new=new[::2]
    err = sorted([abs(val) for val in new-old])[0]
    return err

results = integrate(samples,results)
preset = sp.arctan(samples)
preset_errors = [fabs(x) for x in results-preset]

graph.figure(1)
graph.plot(samples,results,'ro',samples,preset,'k')
graph.legend((r'$quad fn$',r'$tan^{-1}(x)$'))
graph.title(r'$Plot of quad fn and $1/(1+t^{2})$')

graph.figure(2)
graph.semilogy(samples,preset_errors,'ro')

results = series(samples,manual)	
new_samples=samples
new_results=results

# Keep doing until error is below tolerance
while(err>pow(10,-7)):
    counter+=1
    if counter>9:
        break
    new_samples,new_results=ftrap(samples,results)	
    step = step/2
    steps.append(step)
    new_results = series(new_samples,new_results)
    err = error(results,new_results)
    samples=new_samples
    results=new_results
    preset = sp.arctan(new_samples)
    real_errors.append(sorted([fabs(x) for x in new_results-preset])[0])
    estimated_errors.append(err)
    print "Running Ftrap iteration", counter
    
graph.figure(3)
graph.loglog(steps[1:],estimated_errors[1:],'ro',steps[1:],real_errors[1:],'+')
graph.show()


