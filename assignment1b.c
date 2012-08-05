#define SAMPLE_LOWER -3.1416
#define SAMPLE_UPPER 3.1416
#define SAMPLE_SIZE 201
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/*
Plot of 
f(x) = sin(x)+(sin(3x)/3)+(sin(5x)/5)+(sin(7x)/7)
from -pi to pi

Swapnil Basak
EE11B122
*/

float Eval(float x)
{
    return sin(x)+(sin(3*x)/3)+(sin(5*x)/5)+(sin(7*x)/7);
}

int main()
{
    float k=SAMPLE_SIZE;
    int i=0;
    float x[SAMPLE_SIZE];
    float fx[SAMPLE_SIZE];
    float step = (float)((SAMPLE_UPPER-SAMPLE_LOWER)/k);
    FILE *p;
    if((p=fopen("output2.dat","w"))==NULL)
    {
        printf("Cannot Open File!");
        exit(0);
    }
    for(i=0; i<k; i++)
    {      
        x[i] = SAMPLE_LOWER + step*(i);  // The sample values        
        fx[i] = Eval(x[i]);              // Corresponding f(x) values
        fprintf(p,"%f %f \n", x[i], fx[i]);
    }
    fclose(p);
    system("gnuplot -e \'plot \"output2.dat\" w linesp\' -p");

}
