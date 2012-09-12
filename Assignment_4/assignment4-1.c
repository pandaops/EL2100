#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<math.h>

#define BUFLEN 256

typedef struct node
{
  struct node *prev;
  struct node *next;
  int n1,n2,n3,n4;
  char *depname,name[3];
  float value;
}Node;

FILE *fp;
char buf[BUFLEN];// holds line
char *name,*power;
char *word,*pbuf;
char *a[6];
int i,j,n,row;


void read(char**);
double set_exp(char*);		
void print(Node*);
void delete(Node*);
int impedance(char*);
int cccs_ccvs(char*);
int vccs_vcvs(char*);
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

void read(char **argv) 
{ 
  fp=fopen(argv[1],"r");	
  Node *head=(Node*)malloc(sizeof(Node));
  head=NULL;
  while(fgets(buf,BUFLEN-1,fp))
    {
      for( n=0,pbuf=buf ; n<100 ; n++ )
	{
	  if( (word=strtok(pbuf," \t\n")) == NULL )
	    break;
	  a[n]=malloc(sizeof(word));	
	  a[n]=word;
	  pbuf=NULL;	   
	}
      Node *node_buf=(Node*)malloc(sizeof(Node));
      name=malloc(sizeof(a[0]));
      node_buf->next=NULL;
      if(!head)
	node_buf->prev=NULL;
      else
	{
	  node_buf->prev=head;
	  head->next=node_buf;
	}		    	    
      name=a[0];
      if(atof(a[1])>0&&atof(a[2])>0)
	{ 
	  node_buf->n1=atof(a[1]);
	  node_buf->n2=atof(a[2]);
	}
      else
	printf("Negative indexes not supported");      
      if(vccs_vcvs(name))
	{
	  strcpy(node_buf->name,name);  
	  node_buf->n3=atof(a[3]);
	  node_buf->n4=atof(a[4]);
	  node_buf->depname=" ";		   	
	  power=malloc(sizeof(a[5]));
		   
	  sscanf(a[5],"%*f%s",power);
		  
	  node_buf->value=atof(a[5])*set_exp(power);
	}	
      else if(cccs_ccvs(name))
	{
	  strcpy(node_buf->name,name); 
	  node_buf->n3=-1;
	  node_buf->n4=-1;
	  node_buf->depname=a[3];			
	  power=malloc(sizeof(a[4]));				
	  sscanf(a[4],"%*f%s",power);
	  node_buf->value=atof(a[4])*set_exp(power);
	}		
      else if(impedance(name)) 
	{
	  strcpy(node_buf->name,name);   
	  node_buf->n3=-1;
	  node_buf->n4=-1;
	  node_buf->depname=" ";
			
	  power=malloc(sizeof(a[3]));
			
	  sscanf(a[3],"%*f%s",power);
	  node_buf->value=atof(a[3])*set_exp(power);
	}	
      else
      printf("Invalid input");	
      *power=' ';
      head=node_buf;					  		
    }
  head->next=NULL;
  fclose(fp);
  print (head);
}
  	
void print(Node* head)
{
  Node *temp=(Node*)malloc(sizeof(Node));
  temp=head;
	
  while(temp)
    {
      if(vccs_vcvs(temp->name))
	{
	  printf("%s %d %d %d %d %f\n",temp->name,temp->n1,temp->n2,temp->n3,temp->n4,temp->value);
	}	
		
	else if(cccs_ccvs(temp->name))//temp->name[0]=='H'||temp->name[0]=='F'||temp->name[0]=='h'||temp->name[0]=='f')
	{
	  printf("%s %d %d %s %f\n",temp->name,temp->n1,temp->n2,temp->depname,temp->value);
	}	
	  else if(impedance(temp->name))
	{
	  printf("%s %d %d %f\n",temp->name,temp->n1,temp->n2,temp->value);
	}	
				
      temp=temp->prev;
    }
}

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

double set_exp(char *power)
{
  if(*power=='n')
    return pow(10,-9);
  else if(*power=='u')
    return pow(10,-6);
  else if(!strcmp(power,"meg"))
    return pow(10,6);	
  else if(*power=='k')
    return pow(10,3);	
  else if(*power=='m')
    return pow(10,-3);
  else
    return 1;	
}

int impedance(char *name)
{
  if(*name=='R'||*name=='L'||*name=='V'||*name=='C'||*name=='I'||*name=='r'||*name=='l'||*name=='c'||*name=='v'||*name=='i')
    { return 1;
    }
  else return 0;  
}

int cccs_ccvs(char *name){
  if(*name=='H'||*name=='F'||*name=='h'||*name=='f'){
    return 1;
  }
  else return 0;
}

int vccs_vcvs(char *name){
  if(*name=='E'||*name=='G'||*name=='e'||*name=='g'){
    return 1;
  }
  else return 0;
}

//int type(char *name)
//{
// if(!flag=impedance(name)){
//   if(!cccs_ccvs(name)){
//     if(!vccs_vcvs(name)){//
//	return 0;
//     }}}
// return 1;
//}
