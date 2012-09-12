#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

typedef struct Node{
    struct Node *next ;
    int a;
    int b;
}Node;

typedef struct List {
    Node *head ;
    int count ;
}List;

void add(List *list,Node *node){
    node->next = list->head;
    list->head = node;
    list->count+=1;
}

void print(Node *node){
if(node)
    {
    printf("%d %d\n",node->a,node->b);
    print(node->next);        
    }
}

void delete(List *list)
{
Node *tmp = list->head;
if(tmp)
{
    list->head=tmp->next;
    free(tmp);
    delete(list);
    }
}

void main(){
int len;
int val1,val2;
int i=0;
scanf("%d",&len);
Node *new;
List *list = (List *) malloc (sizeof(List)) ;
list->count = 0 ;
list->head=NULL;
for(i=1;i<=len;i++)
{
    scanf("%d%d",&val1,&val2);
    new= (Node *)malloc(sizeof(Node));
    assert(new!=NULL);
    new->next = NULL ;
    new->a=val1;
    new->b=val2;
    add(list,new);
}
print(new);
delete(list);
free(list);
}


