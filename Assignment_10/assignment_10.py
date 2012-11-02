#! usr/bin/python

import sys
from scipy import *
from matplotlib.pyplot import *
from matplotlib.mlab import *

# Take arguements from command line
try:
    n=int(sys.argv[1]) # spatial grid size
    M=int(sys.argv[2]) # number of electrons injected per turn
    nk=int(sys.argv[3]) # number of turns to simulate 
    u0=int(sys.argv[4]) # threshold activity
    p=float(sys.argv[5]) # probability that ionisation will occur
except:
    print "Invalid values"    
    n=100
    M=5
    nk=500
    u0=7 
    p=0.5 
    print "Taking default values n=%d, M=%d, nk=%d, u0=%d, p=%f" %(n,M,nk,u0,p)    
    
xx=zeros(n*M) # Electron Position
u=zeros(n*M) # Electron Velocity
dx=zeros(n*M) # Displacement in current turn

I=[] # Intensity
X=[] # Electron position
V=[] # Electron velocity

for k in range(nk):
    
    # Check for electrons which are in the chamber
    ii=where(xx>0);
    dx[ii]=u[ii]+0.5
    xx[ii]=xx[ii]+dx[ii]
    u[ii]=u[ii]+1
    
    # Check for electrons that have reached the end
    end=where(xx>=n)
    xx[end]=0
    u[end]=0
    dx[end]=0
    
    # Check for electrons having more than critical velocity
    active=where(u>=u0)
    ion=where(rand(len(active[0]))<=p);
    index=active[0][ion]
    u[index]=0
    xx[index]=xx[index]-rand(len(index))*dx[index]
    I.extend(xx[index].tolist())
    
    # Random distribution of injected electrons
    pos=where(xx==0)
    if len(pos[0])<M:
        xx[pos]=1
    else:
        xx[pos[0][:randn()*2+10]]=1
    pos = where(xx>0)  
    
    # Position and velocity
    X.extend(xx[pos].tolist())
    V.extend(u[pos].tolist())
    
# Population plot    
figure(0)
hist(X,bins=n)
xlabel('x')
ylabel('Position')

# Intensity plot
figure(1)
hist(I,bins=n)
xlabel(r"x")
ylabel(r"Intensity")

# Obtaining table from hist and saving
bins=hist(I,bins=n)[1]
xpos=(0.5)*(bins[0:-1]+bins[1:])
count=hist(I,bins=n)[0]
x=hstack((xpos.reshape((n,1)),count.reshape((n,1))))
savetxt("intensity.dat",c_[xpos,count])

# Plotting electron phase space
figure(2)
plot(xx,u,"x")
xlabel('x')
ylabel('velocity')

# Plotting electron phase space (TOTAL)
figure(3)
plot(X,V,"x")
xlabel('x')
ylabel('velocity')

show()
