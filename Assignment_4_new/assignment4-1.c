#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define BUFLEN 256

// The node structure
typedef struct node
{
  struct node *prev;
  struct node *next;
  // Node names cannot excede 5 characters
  char n1[5],n2[5],n3[5],n4[5];
  char *depname,name[3];
  float value;
}Node;

// Global declarations
FILE *fp;
char buf[BUFLEN]; // holds line
char *name,*power;
char *word,*pbuf;
char *a[6];
int i,j,n,row;

// function declarations
void read(char**);
double set_exp(char*);		
void print(Node*);
void delete(Node*);
int impedance(char*);
int cccs_ccvs(char*);
int vccs_vcvs(char*);
// Not used as of now
int type(char*);

int main(int argc,char **argv)
{	
  if( argc<2 )
    {
      printf("Usage: ./a.out filename\n");
      exit(1);
    }
  read(argv);
  return(1);
}

// Main parser
void read(char **argv) 
{ 
  fp=fopen(argv[1],"r");	
  Node *head=(Node*)malloc(sizeof(Node));
  head=NULL;
  // Read till EOF
  while(fgets(buf,BUFLEN-1,fp))
    {
      Node *node_buf=(Node*)malloc(sizeof(Node));
      node_buf->next=NULL;
      name=malloc(sizeof(a[0]));
      // Name of the element
      for( n=0,pbuf=buf ; ; n++ )
	{
	  if( (word=strtok(pbuf," \t\n")) == NULL )
	    break;
	  a[n]=malloc(sizeof(word));	
	  a[n]=word;
	  // pbuf needs to be NULL on after first call of strtok
	  pbuf=NULL;	   
	}
      // Check if this the first node
      if(!head)
	node_buf->prev=NULL;
      else
	{
	  node_buf->prev=head;
	  head->next=node_buf;
	}		    	    
      name=a[0];
      strcpy(node_buf->n1,a[1]);
      strcpy(node_buf->n2,a[2]);
      // Call utility functions to find the type of element. 
      strcpy(node_buf->name,name);
      int scanindex;
      if(vccs_vcvs(name))
	scanindex=5;
      else if(cccs_ccvs(name))		
	scanindex=4;		
      else if(impedance(name)) 
	scanindex=3;
      else
	{
	  printf("Invalid input");	
	  exit(0);
	}
    
      if (scanindex==5)
	{
	  strcpy(node_buf->n3,a[scanindex-2]);
	  strcpy(node_buf->n4,a[scanindex-1]);
	}
      else
	{
	  strcpy(node_buf->n3,"NULL");
	  strcpy(node_buf->n4,"NULL");
	}  
      if (scanindex==4)
	node_buf->depname=a[scanindex-1];
      else
        node_buf->depname=" ";
      // Exponent operations
      power = malloc(sizeof(a[scanindex]));
      sscanf(a[scanindex],"%*f%s",power);
      node_buf->value=atof(a[scanindex])*set_exp(power);
	
      *power=' ';
      head=node_buf;					  		  
    }
    
  // Necessary end conditions
  head->next=NULL;
  fclose(fp);
  print (head);
}

// Prints all the nodes  	
void print(Node* head)
{
  Node *temp=(Node*)malloc(sizeof(Node));
  temp=head;
  // traces the link list with temp
  while(temp)
    {
      if(vccs_vcvs(temp->name))
	{
	  printf("%s %s %s %s %s %f\n",temp->name,temp->n1,temp->n2,temp->n3,temp->n4,temp->value);
	}	
		
      else if(cccs_ccvs(temp->name))
	{
	  printf("%s %s %s %s %f\n",temp->name,temp->n1,temp->n2,temp->depname,temp->value);
	}	
      else if(impedance(temp->name))
	{
	  printf("%s %s %s %f\n",temp->name,temp->n1,temp->n2,temp->value);
	}	
				
      temp=temp->prev;
    }
}

// Frees memory
void delete(Node* head)
{
  Node *temp;
  temp=head;
  while(temp)
    {
      head=temp;
      temp=temp->prev;
      free(head);
    }
			
}

// UTILITY FUNCTIONS

//Returns power based on char formatting
double set_exp(char *power)
{ 
  if (strcmp(power,"meg")==0)
    return pow(10,6);
  switch(*power)
    {
    case 'n': return pow(10,-9);
    case 'k': return pow(10,3);
    case 'm': return pow(10,-3);
    case 'u': return pow(10,-6);
    default: return 1;  
    }
}

// Checks for R, L, C
int impedance(char *name)
{
  switch(*name)
    {
    case 'R':
    case 'L':
    case 'C':
    case 'V':
    case 'I':
    case 'r':
    case 'l':
    case 'c':
    case 'v':
    case 'i': return 1;
    default: return 0;  
    }
}

// Checks for CCVS or CCCS
int cccs_ccvs(char *name){
  switch(*name)
    {
    case 'H':
    case 'F':
    case 'h':
    case 'f': return 1;
    default: return 0;  
    }
}

// Checks for VCVS or VCCS
int vccs_vcvs(char *name){
  switch(*name)
    {
    case 'E':
    case 'G':
    case 'e':
    case 'g': return 1;
    default: return 0;  
    }
}
