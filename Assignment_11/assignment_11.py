#! usr/bin/python

import sys
from pylab import *
from scipy import *
from matplotlib.pylab import *
import numpy as np
import mpl_toolkits.mplot3d.axes3d as p3
from matplotlib.mlab import *
from scipy.linalg import lstsq

if len(sys.argv)<=5:
    print '\n Default values:\n Nx=30\n Ny=30\n Nbegin=8\n Nend=17\n Niter=1500\n'
    Nx=30                       # size along x
    Ny=30                       # size along y
    Nbegin=8                    # start of electrode on each side
    Nend=17                     # end of electrode on each side
    Niter=1500                  # number of iterations to perform
elif len(sys.argv)==6:
    Nx=int(sys.argv[1])
    Ny=int(sys.argv[2])
    Nbegin=int(sys.argv[3])
    Nend=int(sys.argv[4])
    Niter=int(sys.argv[5])

def runsim(Nx,Ny,Nbegin,Nend,Niter):
    phi = zeros((Ny,Nx))		# declare potential array
    phi[0,1:-1] = 1				# top boundary
    phi[-1,1:-1] = 2			# bottom boundary
    phi[1:-1,0] = 3				# left boundary
    phi[1:-1,-1] = 4			# right boundary
    phi[0,Nbegin:(Nend+1)] = 5	# positive electrode
    phi[-1,Nbegin:(Nend+1)] = 6	# negative electrode
    phi[1:-1,1] = -3			# left adjacent
    phi[1:-1,-2] = -4			# right adjacent
	
	# Extracting coordinates
    yb3,xb3 = where(phi==3)
    yb4,xb4 = where(phi==4)
    ya3,xa3 = where(phi==-3)
    ya4,xa4 = where(phi==-4)
        
    phi[1,1:-1] = -1			# top adjacent
    phi[-2,1:-1] = -2			# bottom adjacent
    phi[1,Nbegin:(Nend+1)] = 0	# positive electrode adjacent
    phi[-2,Nbegin:(Nend+1)] = 0	# negative electrode adjacent
	
    # Extracting coordinates
    yb1,xb1 = where(phi==1)
    yb2,xb2 = where(phi==2)
    ya1,xa1 = where(phi==-1)
    ya2,xa2 = where(phi==-2)
    ye1,xe1 = where(phi==5)
    ye0,xe0 = where(phi==6)
    
    phi[:,:] = 0				# Reset the potential arrary
    phi[ye1,xe1] = 1 			# positive potential
    errors=[]
    error=[]
    all_errors=[]	
    
    for k in range(Niter):
        # Copy phi
        oldphi=phi.copy()
        # Each non-border element is the average of the elements around it
        phi[1:-1,1:-1]=0.25*(phi[1:-1,:-2]+ phi[1:-1,2:]+phi[:-2,1:-1]+phi[2:,1:-1])
        # Boundary Conditions
        phi[yb1,xb1]=phi[ya1,xa1]
        phi[yb2,xb2]=phi[ya2,xa2]
        phi[yb3,xb3]=phi[ya3,xa3]
        phi[yb4,xb4]=phi[ya4,xa4]
        # Calculating Max error
        err=abs(phi-oldphi).max()
        all_errors.append(err)
        if k%50==0:
            errors.append(err)
            if k>=500:
                error.append(err)
    # Calculating Jx and Jy 
    Jx=zeros((Ny,Nx))
    Jy=zeros((Ny,Nx))
    Jx[1:Ny-1,1:Nx-1]=0.5*(phi[1:Ny-1,0:Nx-2]-phi[1:Ny-1,2:Nx])
    Jy[1:Ny-1,1:Nx-1]=0.5*(phi[0:Ny-2,1:Nx-1]-phi[2:Ny,1:Nx-1])
    return phi, errors, error, all_errors, Jx, Jy
    
f=runsim(Nx,Ny,Nbegin,Nend,Niter)
# Allocating values
phi=f[0]
errors=f[1]
error=asarray(f[2])
all_errors=asarray(f[3])
Jy=f[5]
Jx=f[4]
# Calculating Ienter, Iexit, Iavg, Idiff
Ienter = sum(Jy[1,:])
Iexit = sum(Jy[-2,:])
Iavg = 0.5*(Ienter + Iexit)
Idiff = abs(Ienter - Iexit)
# The X axis
x=np.asarray(range(len(all_errors)))

# Semilogy Plot with the best fits both for linear and exponential zone
figure(1)
semilogy(x[::50],all_errors[::50],'ro')
x=x.reshape(len(x),1)
x=hstack((ones((len(x),1)),x))
all_errors=all_errors.reshape(len(all_errors),1)
p,resid,rank,sig=lstsq(x,log(all_errors))
semilogy(x,exp(p[0][0]+p[1][0]*x),'b')
K=arange(500,Niter,step=50)
K=K.reshape(len(K),1)
K=hstack((ones((len(K),1)),K))
error=error.reshape(len(error),1)
p,resid,rank,sig=lstsq(K,log(error))
semilogy(K,exp(p[0][0]+p[1][0]*K*log(e)),'k')
legend(('error','fit1','fit2'))
xlabel('No. of iterations $k$')
ylabel(r'$log(error)$')
title(r"Evolution of error with iteration")

# The 3D contour plot as given in the pdf
fig=figure(2)		# open a new figure
ax=p3.Axes3D(fig) 	# Axes3D is the means to do a surface plot
xx=range(Nx)  	    # create x and y axes
yy=range(Ny)
X,Y=meshgrid(xx,yy) # creates arrays out of x and y
title('The 3-D surface plot of the potential')
surf = ax.plot_surface(Y, X, phi, rstride=1, cstride=1, cmap=cm.jet)

# Contour plot of Phi
figure(3)		    
contour(xx,yy,phi,20)
xlabel('x')
ylabel('y')
title(r"Contour plot of $potential phi$ for A and B")

# Quiver plot of Jx and Jy
figure(4)
quiver(yy,xx,Jy.transpose(),Jx.transpose())
title(r"Vector plot of the current flow")

# Empty matrices for the Current and the resistance size
Imatrix=[]
Nymatrix=[]

# Perform 15000 iterations on each resistor length
for i in range(6):
    print "Simulating for Ny=%d, 15000 iterations"%(Ny)
    Jy = runsim(Nx,Ny,Nbegin,Nend,Niter=15000)[5]
    Ienter = sum(Jy[1,:])
    Iexit = sum(Jy[-2,:])
    Iavg = 0.5*(Ienter + Iexit)
    # Store Avg I and Ny
    Imatrix.append(Iavg)
    Nymatrix.append(Ny)
    Ny+=10

# Plot the discrete points of Avg I at Ny and also
# plot the best fit equation
figure(5)
plot(Nymatrix,Imatrix,'ro')
Nymatrix = array(Nymatrix).reshape((6,-1))
Imatrix = array(Imatrix).reshape((6,-1))
logNy = log(Nymatrix)
MlogNy = hstack((ones_like(logNy),logNy))
logI = log(Imatrix)
p = lstsq(MlogNy,logI)[0]
exp = e**p[0]
alphath = p[1]
Ith = exp*(Nymatrix**alphath)
plot(Nymatrix,Ith,'k')
xlabel('length of the resistor')
ylabel('Current')
legend(('Error','$13.184N_y^{-0.896}$'), loc = 0)

show()
