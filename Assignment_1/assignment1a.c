#define SAMPLE_LOWER 0
#define SAMPLE_UPPER 10
#define SAMPLE_SIZE 101
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/* 
   f (x) = x^2 + x sin x / (x + cos x)
   Best fit y=mx+c
   
   Swapnil Basak
   EE11B122
*/


// This evaluates f(x) for each x
float Eval(float x)
{
    float fx=0;
    fx = (x*x+x*sin(x))/(x+cos(x));
    return fx;
} 


int main(int argc, char **argv)
{
    FILE *p;
    if((p=fopen("output1.dat","w"))==NULL)
    {
        printf("Cannot Open File!");
        exit(0);
    }
    int i=0;
    float slope=0;
    float intercept=0;
    float k=SAMPLE_SIZE;
    float xsum = 0;
    float xy = 0;
    float ysum=0;
    float xsqr=0;
    double x[SAMPLE_SIZE];   // The sample values
    double fx[SAMPLE_SIZE];  // Corresponding f(x) values
    float step = (float)((SAMPLE_UPPER-SAMPLE_LOWER)/k);
    // step is calculated before loop 
    for(i=0; i<k; i++)
    {
        x[i] = SAMPLE_LOWER + step*(i);
        xsum+=(x[i]);
        xsqr+=(x[i]*x[i]);
        fx[i] = Eval(x[i]);
        ysum+=fx[i];
        xy+=(x[i]*fx[i]);
    }
    slope = (xy - (xsum * ysum)/k) / (xsqr - (xsum * xsum)/k); // Best fit for m
    intercept = (ysum - (slope*xsum))/k;                       // Best fit for c
    fprintf(p,"# m= %f  c= %f \n",slope, intercept);           // Write to File 
    fprintf(p,"# x f(x) mx+c \n");
    for(i=0; i<k; i++)
    {
        fprintf(p,"%f %f %f \n", x[i], fx[i], (slope*x[i]+intercept)); // Write to file
    }
    fclose(p);
    system("gnuplot -e 'plot \"output1.dat\" u 1:2 w p,\"\" u 1:3 w l' -p"); // Execute system command to plot
}
