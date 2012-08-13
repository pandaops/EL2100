#include<stdio.h>
#include<stdlib.h>
#define MAXLENGTH 512

/*  
    Program to find out no. of words with N
    chars breaking at ',','.','/t','/0',' '
    This is the Pointer method implementation.
    
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
int i = 0;
char wordcount[10];
int wordlen=0;
int ptr_i=0;
int count=0;
// Buffer pointer
char *ptr;
// Initialize all to 0
for(i=0;i<10;i++)
{
    wordcount[i]=0;
}
// Loop till EOF
while(fgets(buf, MAXLENGTH, fp))
{
    printf("Parsing text --> %s \n", buf);
    ptr=buf;
    for(ptr=buf;;ptr++)
    {
        // Split at specified characters and check if it is in between a sentence
        // or at the beginning
        if(*ptr==' '||*ptr=='\t'||*ptr==','||*ptr=='.')
        {
            // Second condition avoids double spaces and spaces after periods
            if((ptr_i!=0)&&((ptr_i-count)>1))
            {
                wordlen=ptr_i-count;
                // Move count to letter after present ptr_i
                count=ptr_i+1;
                wordcount[wordlen]+=1;    
                wordlen=0;
            }
        }
        // Specific logic for EOF as vars need to be reset
        if(*ptr=='\0')
        {
            wordlen=ptr_i-count-1;
            wordcount[wordlen]+=1;    
            wordlen=0;
            ptr_i=0;
            count=0;
            // End Iteration
            break;    
        }
        // Increment ptr_i
        ptr_i++;
    }
}
for(i=1;i<10;i++)
{
    printf("The no. of %d lettered words are %d \n", i, wordcount[i]);    
}           
return 0;
}
