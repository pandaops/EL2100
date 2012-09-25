#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<math.h>

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

struct hash_frag
{
  char node_name[6];
  int index;
};



// Global declarations
FILE *fp;
char buf[BUFLEN]; // holds line
char *name,*power;
char *word,*pbuf;
char *a[6];
int i,j,n,row;

// function declarations
void hash_table(Node*);
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
      for( n=0,pbuf=buf ; n<100 ; n++ )
	{
	  if( (word=strtok(pbuf," \t\n")) == NULL )
	    break;
	  a[n]=malloc(sizeof(word));	
	  a[n]=word;
	  // pbuf needs to be NULL on after first call of strtok
	  pbuf=NULL;	   
	}
      Node *node_buf=(Node*)malloc(sizeof(Node));
      // Name of the element
      name=malloc(sizeof(a[0]));
      node_buf->next=NULL;
      // Check if this the first node
      if(!head)
	node_buf->prev=NULL;
      else
	{
	  node_buf->prev=head;
	  head->next=node_buf;
	}		    	    
      name=a[0];
      // Check if atof[0] exists or it is a string
      if(atof(a[1])>=0&&atof(a[2])>=0)
	{ 
	  strcpy(node_buf->n1,a[1]);
	  strcpy(node_buf->n2,a[2]);
	}
      else
	printf("Negative indexes not supported"); 
      // Call utility functions to find the type of element. Switch case implementation 
      // will be better?     
      if(vccs_vcvs(name))
	{
	  strcpy(node_buf->name,name);  
	  strcpy(node_buf->n3,a[3]);
	  strcpy(node_buf->n4,a[4]);
	  node_buf->depname=" ";		   	
	  power=malloc(sizeof(a[5]));
	  sscanf(a[5],"%*f%s",power);
	  node_buf->value=atof(a[5])*set_exp(power);
	}	
      else if(cccs_ccvs(name))
	{
	  strcpy(node_buf->name,name); 
	  strcpy(node_buf->n3,"-1");
	  strcpy(node_buf->n4,"-1");
	  node_buf->depname=a[3];			
	  power=malloc(sizeof(a[4]));				
	  sscanf(a[4],"%*f%s",power);
	  node_buf->value=atof(a[4])*set_exp(power);
	}		
      else if(impedance(name)) 
	{
	  strcpy(node_buf->name,name);   
	  strcpy(node_buf->n3,"-1");
	  strcpy(node_buf->n4,"-1");
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
  // Necessary end conditions
  head->next=NULL;
  fclose(fp);
  hash_table(head);
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

// Checks for R, L, C
int impedance(char *name)
{
  if(*name=='R'||*name=='L'||*name=='V'||*name=='C'||*name=='I'||*name=='r'||*name=='l'||*name=='c'||*name=='v'||*name=='i')
    { return 1;
    }
  else return 0;  
}

// Checks for CCVS or CCCS
int cccs_ccvs(char *name){
  if(*name=='H'||*name=='F'||*name=='h'||*name=='f'){
    return 1;
  }
  else return 0;
}

// Checks for VCVS or VCCS
int vccs_vcvs(char *name){
  if(*name=='E'||*name=='G'||*name=='e'||*name=='g'){
    return 1;
  }
  else return 0;
}

// Make it more modular and call only this function?
//int type(char *name)
//{
// if(!flag=impedance(name)){
//   if(!cccs_ccvs(name)){
//     if(!vccs_vcvs(name)){//
//	return 0;
//     }}}
// return 1;
//}

void hash_table(Node* node)
{
  struct hash_frag hash_table[50];
  Node *tmp;
  tmp = node;
  int len=0;
  int count=1;
  for(len=1;;)
    {
      tmp=tmp->prev;
      len+=1;
      if(tmp->prev==NULL)
	break;
    }
  //hash_table=(struct hash_frag*)malloc(sizeof(struct hash_frag)*len);
  for(i=0;i<50;i++)
    {
      strcpy(hash_table[i].node_name,"0");
      hash_table[i].index=0;
    }
  int exists_n1=0;  
  int exists_n2=0;  
  tmp=node;
  for(j=0;j<len;j++)
    {  
      if(tmp==NULL)
	break;      
      for(i=0;i<=count;i++)
	{
	 if(strcmp(hash_table[i].node_name,tmp->n1)==0)
	    {
	      exists_n1=1;
	    }  
         if(strcmp(hash_table[i].node_name,tmp->n2)==0)
	    {
	      exists_n2=1;
	    }
	}
      if(!exists_n1)
	{
	  strcpy(hash_table[count].node_name, tmp->n1);
	  hash_table[count].index=count;
	  count++;
	}
      if(!exists_n2)
	{
	  strcpy(hash_table[count].node_name, tmp->n2);
	  hash_table[count].index=count;	      
	  count++;
	}	
      tmp=tmp->prev;
      exists_n1=0;
      exists_n2=0;
    }
  printf("Generated Hash Table \n");
  for(i=0;i<count;i++){
    printf("index=%d          name=%s\n",hash_table[i].index,hash_table[i].node_name);
  }
  generate_matrix(count,count,tmp);
}

void generate_matrix(int count,int elem, Node* node)
{
  int i,j,k;
  complex **G,*I1,**R;
  G=malloc(count*sizeof(complex));
  for(i=0;i<sizeofmat;i++)											
	{
		for(j=0;j<sizeofmat;j++)
			G[i][j]=0;
	}			
	
I1=malloc(sizeofmat*sizeof(complex));
for(i=0;i<sizeofmat;i++)
*(I1+i)=0; 

