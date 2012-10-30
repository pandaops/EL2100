# script to generate data files for Week 8
# assignment in Python
from scipy import *
from matplotlib.pyplot import *
import scipy.special as sp
N=101
k=9
# generate the data points and add noise
t=linspace(0,10,N)
y=sp.jn(2,t)-0.02546303*t
y=reshape(y,(N,1))
scl=logspace(-1,-3,k)
n=dot(randn(N,k),diag(scl))
yy=dot(y,ones((1,k)))+n
# shadow plot
#plot(t,yy)
#show()
z=zeros((N,k+1))
z[:,0]=t
z[:,1:k+1]=yy
savetxt("fitting.dat",z)
