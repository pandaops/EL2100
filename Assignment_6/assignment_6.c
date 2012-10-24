#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#define BUFLEN 256

/*******************************************************************************
 *                                                                             
 * This is a program that handles and analyzes a electrical circuit. It involves 
 * five different steps/modules.  
 * 
 * PART 1
 *
 * It first parses a text file for information on the circuit and then converts 
 * verbose inputs to numerical ones. A restriction on the number of characters
 * that a node can contain is restricted(=5). This module mainly improves UI and
 * allows for quick and readable input.
 *
 * PART 2
 * 
 * It then forms a hash table to index node names, which also accounts for 
 * alphanumeric node names. It then returns the respective index to part 3, which
 * uses it to make the conductance matrix.
 *
 * PART 3
 * 
 * It calls part 2 for each element it finds and correspondingly adds the value 
 * to the matrix cell. It then compiles the resultant matrix with other details
 * such as independent voltage sources and current sources. It outputs the 
 * Conductance Matrix and the Current Matrix in the equation G*V=I, using 
 * standard sign conventions.
 * 
 * PART 4
 *  
 * It finds the co-factor for each entry [i][j]. It stores the values of the
 * cofactor of [i][j] in another similar martix. It then finds the inverse of
 * the matrix using the cofactor identity and calls the gaussian_determinant 
 * function recursively.
 *
 * PART 5
 *       
 * This calls the gaussian_determinant function in accordance to the algorithm of 
 * gaussian elimination. Finally, the inverse of the matrix is found, multiplied 
 * with the original cur_mat and displayed.
 *
 *
 * -Swapnil Basak
 *  EE11B122                                                                 
 ******************************************************************************/

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
int sizeofimp_mat;
struct hash_frag hash_table[50];

// function declarations
void make_hash_table(Node*);
void read(char**);
double set_exp(char*);		
void print(Node*);
void delete(Node*);
int impedance(char*);
int cccs_ccvs(char*);
int vccs_vcvs(char*);
void generate_matrix(int,int,Node*,int);
int find_hash(char*);

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
// Check for type of dependence - current or voltage
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
  make_hash_table(head);
}

// Prints all the nodes  	
void print(Node* head)
{
  Node *temp=(Node*)malloc(sizeof(Node));
  temp=head;
  // traces the link list with temp
  printf("\nNode details : \n\n");
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
  switch(*name)
    {
    case 'R':
    case 'r':
    case 'L':
    case 'l':
    case 'C':
    case 'c':
    case 'V':
    case 'v':
    case 'I':
    case 'i':
      return 1;
    default:
      return 0;
     
    }
}
// Checks for CCVS or CCCS
int cccs_ccvs(char *name){
  switch(*name)
    {
    case 'H':
    case 'h':
    case 'F':
    case 'f':
      return 1;
    default:
      return 0;
     
    }
}

// Checks for VCVS or VCCS
int vccs_vcvs(char *name){
  switch(*name)
    {
    case 'E':
    case 'G':
    case 'e':
    case 'g':
      return 1;
    default:
      return 0;
     
    }
 
}

// Makes hash table for the input

void make_hash_table(Node* node)
{
  Node *tmp;
  tmp = node;
  int len=0;
  int count=0;
  // Total no of elements
  for(len=1;;)
    {
      tmp=tmp->prev;
      len+=1;
      if(tmp->prev==NULL)
	break;
    }
  // Initialisation  
  for(i=0;i<50;i++)
    {
      strcpy(hash_table[i].node_name,"0");
      hash_table[i].index=0;
    }
  int exists_n1=0;  
  int exists_n2=0;  
  tmp=node;
  // hashing function
  // Assign index to node name if not already present
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
    // Output
  printf("\nGenerated Hash Table \n\n");
  printf("+------------------+\n");
  for(i=0;i<count;i++){
    printf("|index=%d | name=%s|\n",hash_table[i].index,hash_table[i].node_name);
  }
  printf("+------------------+\n\n");
  sizeofimp_mat=count;
  tmp=node;
  while(tmp->prev!=NULL)
    {
      if(*(tmp->name)=='V'||*(tmp->name)=='v')
	count++;
      tmp= tmp->prev;
    }
      
  generate_matrix(count-1,sizeofimp_mat,node,1000);
}


 /*****************************************************************************
 * The following part of the program has three utility functions and does
 * three main functions: 
 * 
 * It finds the co-factor for each entry [i][j]. It stores the values of the
 * cofactor of [i][j] in another similar martix. 
 *
 * It then finds the inverse of
 * the matrix using the cofactor identity and calls the gaussian_determinant 
 * function recursively.
 *      
 * This calls the gaussian_determinant function in accordance to the algorithm of 
 * gaussian elimination. Finally, the inverse of the matrix is found, multiplied 
 * with the original cur_mat and displayed.
 *                                                              
 ******************************************************************************/



complex gaussian_determinant(complex **con_mat,int sizeofmat )									
{
	float parity=1;
	complex	value=0,**b;
	int i,j,m,n,c;
	
	b=malloc((sizeofmat)*sizeof(complex));								
	for(i=0;i<sizeofmat;i++)
		b[i]=malloc(sizeofmat*sizeof(complex));
	
	if(sizeofmat==1)
	{
		return(con_mat[0][0]);
	}
	else
	{
		value=0;
		for(c=0;c<sizeofmat;c++)
		{
			m=0;
			n=0;

			for(i=0;i<sizeofmat;i++)
			{
				for(j=0;j<sizeofmat;j++)
				{
					b[i][j]=0;

					if(i!=0&&j!=c)
					{
						b[m][n]=con_mat[i][j];

						if(n<sizeofmat-2)
							n++;
						else
						{
							n=0;
							m++;
						}
					}
				}
			}
			value=value+parity*(con_mat[0][c]*gaussian_determinant(b,sizeofmat-1));				
			parity=-1*parity;
		}
	}
	return(value);												
}

complex** find_inverse(complex **con_mat,complex **gauss_factors,int sizeofmat)				
{
	int i,j;
	complex **b,**inverse,d;
	
	b=malloc((sizeofmat)*sizeof(complex));								
	for(i=0;i<sizeofmat;i++)
		b[i]=malloc(sizeofmat*sizeof(complex));
		
	inverse=malloc((sizeofmat)*sizeof(complex));								
	for(i=0;i<sizeofmat;i++)
		inverse[i]=malloc(sizeofmat*sizeof(complex));	
	
	for(i=0;i<sizeofmat;i++)
	{
		for(j=0;j<sizeofmat;j++)
		{
			b[i][j]=gauss_factors[j][i];
		}
	}
	
	d=gaussian_determinant(con_mat,sizeofmat);
	
	for(i=0;i<sizeofmat;i++)
	{
		for(j=0;j<sizeofmat;j++)
		{
			inverse[i][j]=b[i][j]/d;
		}
	}
	return inverse;													
}
complex** cofactor(complex **con_mat,int sizeofmat)							
{
	complex **gauss_factors,**b,**inverse;
	int p,q,m,n,i,j;
	
	gauss_factors=malloc((sizeofmat)*sizeof(complex));								
	for(i=0;i<sizeofmat;i++)
		gauss_factors[i]=malloc(sizeofmat*sizeof(complex));
		
	b=malloc((sizeofmat)*sizeof(complex));								
	for(i=0;i<sizeofmat;i++)
		b[i]=malloc(sizeofmat*sizeof(complex));
		
	inverse=malloc((sizeofmat)*sizeof(complex));								
	for(i=0;i<sizeofmat;i++)
		inverse[i]=malloc(sizeofmat*sizeof(complex));	
	
	for(q=0;q<sizeofmat;q++)
	{
		for(p=0;p<sizeofmat;p++)
		{
			m=0;
			n=0;
			for(i=0;i<sizeofmat;i++)
			{
				for(j=0;j<sizeofmat;j++)
				{
					b[i][j]=0;
					if(i!=q&&j!=p)
					{
						b[m][n]=con_mat[i][j];
						
						if(n<(sizeofmat-2))
							n++;
						else
						{
							n=0;
							m++;
						}
					}	
				}
			}
			if ((q+p)%2==0)
			gauss_factors[q][p]=gaussian_determinant(b,sizeofmat-1);
		    else
		    gauss_factors[q][p]=-1*gaussian_determinant(b,sizeofmat-1);
		    
		    }
	}
	inverse=find_inverse(con_mat,gauss_factors,sizeofmat );
	return inverse;
}

complex* solution(complex** inverse,complex* cur_mat,int sizeofmat)			
{
	int i,j,k;
	complex* ans_mat;
	ans_mat=malloc((sizeofmat)*sizeof(complex));
	
	for(i=0;i<sizeofmat;i++)
		ans_mat[i]=0;
	
	for(i=0;i<sizeofmat;i++)
	{
		for(j=0;j<sizeofmat;j++)
			ans_mat[i]+=inverse[i][j]*cur_mat[j];
	}		
		
	return ans_mat;
}	
	


/*******************************************************************************
 *                                                                             
 * This is one of the main functions of the program. It does the following:
 * 
 * It initializes three matrices based on input size. 
 * con_mat = Conductance Matrix
 * cur_mat = Current Matrix
 * imp_map = Impedance Matrix (con_mat^-1)
 *                                     
 * -> It first forms the Impedance Matrix usind nodal analyis.
 * -> It then checks for voltage sources and adds current variables correspodingly
 * -> It checks for current sources and adds them to the current matrix
 * 
 * The above is done with the find_hash() function which returns the index
 * for the respective node_name.                                     
 ******************************************************************************/

void generate_matrix(int sizeofmat,int node_max,Node* node1,int w)
{
  int i,j,k;
  complex **con_mat,*cur_mat,**imp_mat,**inverse,*ans_mat;
  complex det,z;		
  con_mat=malloc(sizeofmat*sizeof(complex));	
  for(i=0;i<sizeofmat;i++)
    con_mat[i]=malloc(sizeofmat*sizeof(complex));	
  for(i=0;i<sizeofmat;i++)		
    {
      for(j=0;j<=sizeofmat-1;j++)
	con_mat[i][j]=0;
    }			
  cur_mat=malloc(sizeofmat*sizeof(complex));	
  for(i=0;i<=sizeofmat-1;i++)		
    *(cur_mat+i)=0;
  imp_mat=malloc(node_max*sizeof(complex));	
  for(i=0;i<node_max;i++)		
    imp_mat[i]=malloc(sizeofmat*sizeof(complex));
  Node *node2=(Node*)malloc(sizeof(Node));						
  node2=node1;										
  while(node1)														
    {
      if(*(node1->name)=='R'||*(node1->name)=='r')
	{
	  j=find_hash(node1->n1);
	  k=find_hash(node1->n2);
	  imp_mat[j][k]=node1->value;
	  imp_mat[k][j]=node1->value;	  
	}	
				
      else if(*(node1->name)=='L'||*(node1->name)=='l')
	{
	  j=find_hash(node1->n1);
	  k=find_hash(node1->n2);
	  imp_mat[j][k]=(1/I*w*node1->value);
	  imp_mat[k][j]=I*w*node1->value;
	}
		
      else if(*(node1->name)=='C'||*(node1->name)=='c')
	{
	  j=find_hash(node1->n1);
	  k=find_hash(node1->n2);
	  imp_mat[j][k]=1.0/(I*w*node1->value);
	  imp_mat[k][j]=1.0/(I*w*node1->value);
	}				
      node1=node1->prev;	
    }			 
	
  for(i=0;i<node_max-1;i++)								
    {
      for(j=0;j<i+1;j++)
  	{
	  if(i==j)							
  	    {
  	      for(k=0;k<node_max;k++)
  		{
  		  if(imp_mat[i][k]!=0)							
  		    con_mat[i][j]+=1.0/imp_mat[i][k];					
  		}
  	    }
  	  else
  	    {
  	      if(imp_mat[i][j]!=0)	
  		{
		  con_mat[i][j]=-1/imp_mat[i][j];									
  		  con_mat[j][i]=con_mat[i][j];					
  		}
  	      else
  		{
  		  con_mat[i][j]=0;								
		  con_mat[j][i]=0;
  		}	
  	    }	
  	}
    }	
	
  node1=node2;										

  i=0;
	
  while(node2)										
    {											
      if(*(node2->name)=='V'||*(node1->name)=='v')								
  	{										
  	  j=find_hash(node2->n1);
  	  k=find_hash(node2->n2);
  	  con_mat[node_max+i-1][j]=-1;
  	  con_mat[j][node_max+i-1]=-1;
	  con_mat[node_max+i-1][k]=-1;
  	  con_mat[k][node_max+i-1]=-1;			
  	  cur_mat[node_max+i-1]=node2->value;  					
  	  i++;
  	}
      node2=node2->prev;
    }	
  node2=node1;
	
  while(node1)				
    {
      if(*(node1->name)=='I'||*(node1->name)=='i')
	{
	  j=find_hash(node1->n1);
	  k=find_hash(node1->n2);
	  if(j<node_max-1)
	    cur_mat[j]+=node1->value;				
	  if(k<node_max-1)	    
	    cur_mat[k]+=-1*node1->value;
	}	
      node1=node1->prev;
    }
  printf("The Variables are \n");
  for(i=0;i<sizeofmat;i++)
    {             
      if(i<node_max-1)
	printf("V%d",i+1);
      else
	printf("I%d",i-node_max+2);		
      printf(" ");
    }
  printf("\n\n");	
  printf("The Conductance Matrix: \n \n");	
  for(i=0;i<sizeofmat;i++)
    {
      for(j=0;j<sizeofmat;j++)
	printf("|\t%4.2f+%4.2fi\t",creal(con_mat[i][j]),cimag(con_mat[i][j]));			
      printf("|\n");
    }  		
  printf("\n\nThe Current Matrix:\n\n");
  for(i=0;i<sizeofmat;i++)
    {  		
      printf("|%4.2f+%4.2fi\t|\n",creal(cur_mat[i]),cimag(cur_mat[i]));
    }		
  printf("\n\n");
  
  inverse=cofactor(con_mat,sizeofmat);												
	
	ans_mat=solution(inverse,cur_mat,sizeofmat);
	printf("The solutions to the above circuit are: \n");
	if(sizeofmat!=1)
	{
		for(i=0;i<sizeofmat;i++)
		{
			if(i<node_max-1)
				printf("V%d\t=\t",i+1);
			else
				printf("I%d\t=\t",i-node_max+2);		
		
			printf("%f+%fi\n",creal(ans_mat[i]),cimag(ans_mat[i]));
		}
	}
	
	if(sizeofmat==1)
	{
		z=cur_mat[0]/con_mat[0][0];
		printf("V1\t=\t%f+%fi\n",creal(z),cimag(z));
	}	

  printf("\n");
  free(imp_mat);			
  free(con_mat);
  free(cur_mat);
  free(ans_mat);
  free(inverse);
}
	
// Utility function to lookup hash table	
int find_hash(char* name)
{
  for(i=0;i<sizeofimp_mat;i++)
    {
      if(strcmp(hash_table[i].node_name,name)==0)
	{
	  return i;
	}
    }
  return 0;
}

