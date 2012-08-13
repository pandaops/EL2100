#include<stdio.h>
#include<stdlib.h>
#define MAXLENGTH 512

/*  
    Program to find out no. of words with N
    chars breaking at ',','.','/t','/0',' '
    This is the Array method implementation.
    
    Swapnil Basak  
    EE11B122
*/

int main(int argc, char **argv)
{
// Expects a filename too, check for that
if(argc != 2)
{
    printf("Usage ./a.out <filename>");
    exit(1);
}
// File pointer
FILE *fp;
fp=fopen(argv[1], "r");
if(fp == NULL) 
{
    printf("File could not be opened");
    exit(2);
}
char buf[MAXLENGTH];
int counter = 0;
char wordcount[10];
int wordlen=0;
int tempcount=0;
// Initialize all to 0
for(counter=0;counter<11;counter++)
{
    wordcount[counter]=0;
}
// Loop till EOF
while(fgets(buf, MAXLENGTH, fp))
{
    printf("Parsing text line --> %s", buf);
    for(counter=0;counter<MAXLENGTH;counter++)
    {
        // Split at specified characters and check if it is in between a sentence
        // or at the beginning
        if(buf[counter]==' '||buf[counter]=='\t'||buf[counter]==',')
        {
            // Second condition avoids double spaces and spaces after periods
            if(counter!=0)//&&((counter-tempcount)>1))
            {
                wordlen=counter-tempcount;
                wordcount[wordlen]+=1;    
                wordlen=0;
            }
            // Move tempcount to present space+1
            tempcount=counter+1;
        }
        // Specific logic for EOF as vars need to be reset
        if(buf[counter]=='\0')
        {
            wordlen=counter-tempcount-1;
            wordcount[wordlen]+=1;    
            wordlen=0;
            tempcount=0;
            // End iteration
            break;
        }
    }
}
for(counter=1;counter<10;counter++)
{
    printf("\nResults: \nThe no. of %d lettered words are %d \n", counter, wordcount[counter]);    
}           
return 0;
}
