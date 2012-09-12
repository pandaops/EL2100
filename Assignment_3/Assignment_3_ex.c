#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
typedef struct Node
{
	int a;
	int b;
	struct Node *next;
}Node;

typedef struct List
{
	Node *head;
	int count;
}List;

int len;

void add(List *list,Node *node)
{
	list->count++;
	scanf("%d%d",&node->a,&node->b);
	if(list->count!=len)
	{
		node->next=(Node *)malloc(sizeof(Node));
		assert(node->next!=NULL);
		add(list,node->next);
	}
	else
		node->next=NULL;    
}
void print(Node *node)
{
	if(node)
	{
		printf("%d %d\n",node->a,node->b);
		print(node->next);
	}
}
void delete(List *list)
{
	Node *tmp=list->head;
	if(tmp)
	{
		list->head=tmp->next;
		free(tmp);
		delete(list);
	}
}


void main()
{
	List *list=(List *)malloc(sizeof(List));
	list->count=0;
	scanf("%d",&len);
	Node *new=(Node *)malloc(sizeof(Node));
	add(list,new);
	print(new);
	delete(list);
	free(list);
}

