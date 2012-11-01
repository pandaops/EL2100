import sys
from pylab import *
from scipy import *
from matplotlib.pylab import *
import mpl_toolkits.mplot3d.axes3d as p3
from matplotlib.mlab import *
from scipy.linalg import lstsq

if len(sys.argv)<=5:
    print '\n Default values:\n Nx=25\n Ny=25\n Nbegin=8\n Nend=17\n Niter=1500\n'
    Nx=30            #size along x
    Ny=30             #size along y
    Nbegin=8          #start of electrode on each side
    Nend=17           #end of electrode on each side
    Niter=1500        #number of iterations to perform
elif len(sys.argv)==6:
    Nx=int(sys.argv[1])
    Ny=int(sys.argv[2])
    Nbegin=int(sys.argv[3])
    Nend=int(sys.argv[4])
    Niter=int(sys.argv[5])

def runsim(Nx,Ny,Nbegin,Nend,Niter):
    phi=zeros((Ny,Nx))
    phi[0,Nbegin:Nend]=1
    errors=[]
    error=[]
    all_errors=[]	
    
    for k in range(Niter):
        oldphi=phi.copy()
        phi[1:-1,1:-1]=0.25*(phi[1:-1,:-2]+ phi[1:-1,2:]+phi[:-2,1:-1]+phi[2:,1:-1])
        phi[1:-1,-1]=phi[1:-1,-2]
        phi[1:-1,0]=phi[1:-1,1]
        phi[0,1:-1]=phi[1,1:-1]
        phi[-1,1:-1]=phi[-2,1:-1]
        err=abs(phi-oldphi).max()
        all_errors.append(err)
        if k%50==0:
            errors.append(err)
            if k>=500:
                error.append(err) 
    Jx=zeros((Ny,Nx))
    Jy=zeros((Ny,Nx))
    Jx[1:Ny-1,1:Nx-1]=0.5*(phi[1:Ny-1,0:Nx-2]-phi[1:Ny-1,2:Nx])
    Jy[1:Ny-1,1:Nx-1]=0.5*(phi[0:Ny-2,1:Nx-1]-phi[2:Ny,1:Nx-1])
    Jx[1:-1,0]=Jx[1:-1,1]
    Jx[1:-1,-1]=Jx[1:-1,-2]
    Jy[0,1:-1]=Jy[1,1:-1]
    Jy[-1,1:-1]=Jy[-2,1:-1]
    return phi, errors, error, all_errors, Jx, Jy
    
f=runsim(Nx,Ny,Nbegin,Nend,Niter)
phi=f[0]
errors=f[1]
error=asarray(f[2])
all_errors=asarray(f[3])
Jy = f[5]
Jx=f[4]
x=linspace(1,len(all_errors),len(all_errors))

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

semilogy(K,exp(p[0][0]+p[1][0]*K*log(e)),'+')

legend(('error','fit1','fit2'))

xlabel('No. of iterations $k$')
ylabel(r'$log(error)$')
title(r"Evolution of error with iteration")

fig=figure(2)		# open a new figure
ax=p3.Axes3D(fig) 	# Axes3D is the means to do a surface plot
xx=arange(0,Nx)  	# create x and y axes
yy=arange(0,Ny)
X,Y=meshgrid(xx,yy) 	# creates arrays out of x and y
title('The 3-D surface plot of the potential')
surf = ax.plot_surface(Y, X, phi, rstride=1, cstride=1, cmap=cm.jet)

figure(3)		#contour plot
contour(xx,yy,phi,20)
xlabel('x')
ylabel('y')
title(r"Contour plot of $potential phi$ for A and B")

figure(4)
quiver(yy,xx,Jy.transpose(),Jx.transpose())
title(r"Vector plot of the current flow")
show()


