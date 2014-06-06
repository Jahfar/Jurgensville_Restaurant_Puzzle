# include "stdio.h"
#include "string.h"
#include "stdlib.h"
         
/* structures  */

//structure #1
/* NODE will store available restaurants and prices over there for an item */
struct NODE 
{
	char word[50];
	int rest_id[10];
	float price[10];
	int tag_no[10];
	int no_rest;
	struct NODE *next;
} *head=NULL;

//structure #2
/* TNode is actually tree node corresponding to NODE */
struct TNode
{
	char word[50];
	int rest_id[10];
	float price[10];
	int tag_no[10];
	int no_rest;
        struct TNode* left;
        struct TNode* right;
};

//structure #3
/* restaurants stores available restaurant ID's for the required items */
struct restaurants
{ 
	int no_rest;
	int rest_id[10];
};

////structure #4
/* availableRestaurant stores a available restaurant ID and the prices of required items over there along with it's tag number */
struct availableRestaurant
{ 
	int rest_id;
	int tag_no[10];
	float price[10];
};

/* functions */

//function #1
/* Read data from a csv file and write into a linked list,this function read unlimited rows and around 10 items from a row  */
struct NODE* createLinkedList(char *a)
{	
	FILE *data;
	data = fopen(a, "r");
	char parsedLine[500];
	int i=0,n=0;
	while (fgets(parsedLine, 500, data) != NULL)
	{
	    	struct NODE *node = malloc(sizeof(struct NODE));
	    	char *getRest_id = strtok(parsedLine, ", ");
	    	node->rest_id[0] = atoi(getRest_id);
	    	char *getPrice = strtok(NULL, ", "); 
	    	node->price[0] = atof(getPrice);
	    	char *getWord = strtok(NULL, ", ");
	    	strcpy(node->word, getWord);
		n=strlen(node->word);
		if(node->word[n-1] =='\n') { node->word[n-2]='\0';}
	    	node->tag_no[0]=i;
		node->no_rest=1;
	    	node->next = head;
	    	head = node; 
	    	while( (getWord = strtok(NULL, ", ") ) != NULL)
		{
		    	struct NODE *node = malloc(sizeof(struct NODE));
	    		strcpy(node->word, getWord);
			n=strlen(node->word);
			if(node->word[n-1] =='\n') { node->word[n-2]='\0';}
	   		node->rest_id[0] = atoi(getRest_id);
	    		node->price[0] = atof(getPrice);
	    		node->tag_no[0]=i;
			node->no_rest=1;
	    		node->next = head;
	    		head = node; 
		}
		i++;
   
	}
	fclose(data);
	return(head);
}

/* following functions are used for sorting the list using merge sort */
/* sorts the linked list by changing next pointers (not data) */

//function #2
/* Split the nodes of the given list into front and back halves,
     and return the two lists using the reference parameters.
     If the length is odd, the extra node should go in the front list.
     Uses the fast/slow pointer strategy.  */
void FrontBackSplit(struct NODE* source,struct NODE** frontRef, struct NODE** backRef)
{
	struct NODE* fast;
	struct NODE* slow;
	if (source==NULL || source->next==NULL)
	{
		/* length < 2 cases */
		*frontRef = source;
		*backRef = NULL;
	}
	else
	{
		slow = source;
		fast = source->next;

		/* Advance 'fast' two nodes, and advance 'slow' one node */
		while (fast != NULL)
		{
			fast = fast->next;
			if (fast != NULL)
			{
				slow = slow->next;
				fast = fast->next;
			}
		}

		/* 'slow' is before the midpoint in the list, so split it in two
		at that point. */
		*frontRef = source;
		*backRef = slow->next;
		slow->next = NULL;
	}
}

//function #3
struct NODE* SortedMerge(struct NODE* a, struct NODE* b)
{
	int cond;
	int i=0;
	struct NODE* result = NULL;

	/* Base cases */
	if (a == NULL)
	return(b);
	else if (b==NULL)
	return(a);

	/* Pick either a or b, and recur */
	if((cond=strcmp(a->word,b->word)) <0 )
	{
		result = a;
		result->next = SortedMerge(a->next, b);
	}
	else if ((cond=strcmp(a->word,b->word))==0 )
	{
		for(i=0;i<(b->no_rest);i++)
		{
			a->rest_id[(a->no_rest)+i]=b->rest_id[i];
			a->price[(a->no_rest)+i]=b->price[i];
			a->tag_no[(a->no_rest)+i]=b->tag_no[i];
		}
		a->no_rest+=b->no_rest;
		result=a;
		result->next = SortedMerge(a->next, b->next);
	}
	else
	{
		result = b;
		result->next = SortedMerge(a, b->next);
	}
	return(result);
}

//function #4

void MergeSort(struct NODE** headRef)
{
  struct NODE* head = *headRef;
  struct NODE* a;
  struct NODE* b;
 
  /* Base case -- length 0 or 1 */
  if ((head == NULL) || (head->next == NULL))
  {
	return;
  }
 
  /* Split head into 'a' and 'b' sublists */
  FrontBackSplit(head, &a, &b);
 
  /* Recursively sort the sublists */
  MergeSort(&a);
  MergeSort(&b);
 
  /* answer = merge the two sorted lists together */
  *headRef = SortedMerge(a, b);
}

// following functions are used for converting the sorted list into a balanced binary search tree

//function #5
/* A Binary Tree node */
/* Helper function that allocates a new node with the
   given data and NULL left and right pointers. */
struct TNode* newNode(struct NODE* any_node)
{
	int i=0;
	struct TNode* node = (struct TNode*)malloc(sizeof(struct TNode));
	strcpy(node->word,any_node->word);
	for(i=0;i<any_node->no_rest;i++)
	{
		node->rest_id[i] = any_node->rest_id[i];
		node->price[i] = any_node->price[i];
		node->tag_no[i] = any_node->tag_no[i];
	}
	node->no_rest = any_node->no_rest;
	node->left = NULL;
	node->right = NULL;
	return node;
}

//function #6
/* A function that returns count of nodes in a given Linked List */
int countLNodes(struct NODE *head)
{
	int count = 0;
	struct NODE *temp = head;
	while(temp)
	{
		temp = temp->next;
		count++;
	}
	return count;
}

//function #7
/* The main function that constructs balanced BST and returns root of it.
       head_ref -->  Pointer to pointer to head node of linked list
       n  --> No. of nodes in Linked List */
struct TNode* sortedListToBSTRecur(struct NODE **head_ref, int n)
{
	/* Base Case */
	if (n <= 0)
	return NULL;

	/* Recursively construct the left subtree */
	struct TNode *left = sortedListToBSTRecur(head_ref, n/2);

	/* Allocate memory for root, and link the above constructed left
	subtree with root */
	struct TNode* root = newNode(*head_ref);
	root->left = left;

	/* Change head pointer of Linked List for parent recursive calls */
	*head_ref = (*head_ref)->next;

	/* Recursively construct the right subtree and link it with root
	The number of nodes in right subtree  is total nodes - nodes in
	left subtree - 1 (for root) which is n-n/2-1*/
	root->right = sortedListToBSTRecur(head_ref, n-n/2-1);

	return root;
}

//function #8
/* This function counts the number of nodes in Linked List and then calls
   sortedListToBSTRecur() to construct BST */
struct TNode* sortedListToBST(struct NODE *head)
{
	/*Count the number of nodes in Linked List */
	int n = countLNodes(head);
	/* Construct BST */
	return sortedListToBSTRecur(&head, n);
}

//function #9
/*This function do a binary search of the given name and return the node which has the given name or else it will returns NULL*/
struct TNode* doBinarySearch(struct TNode* root, char *item_name)
{	
	if(root==NULL) return NULL;
	int n=strcmp(item_name,root->word);
	if(n==0) return root;
	else if(n<0) doBinarySearch(root->left,item_name);
	else doBinarySearch(root->right,item_name);	
}

//function #10
/*A function which returns the list of available restaurant ID's which will provide the required  items*/
struct restaurants* availableRestaurants(struct TNode** items,int argc)
{       
	struct restaurants* node = (struct restaurants*)malloc(sizeof(struct restaurants));
	node->no_rest=0;
	int i=0,j=0,k=0;
	int searchTherestIdInTheItem(struct TNode* node ,int rest_id)
	{	
		int i=0,j=0;
		for(i=0;i<node->no_rest;i++)
		{
			 if(node->rest_id[i]==rest_id) 
			 {
				j=1; return(j);  break;
			 }
		}
		if(i==node->no_rest) return(j);
	}
	for(j=0;j<items[0]->no_rest;j++)
	{	
		for(i=1;i<(argc-2);i++)
		{
			k=searchTherestIdInTheItem(items[i],items[0]->rest_id[j]);
			if(k==0) break;
		}
		if(i==argc-2) 
		{
			node->rest_id[node->no_rest]=items[0]->rest_id[j];			
			node->no_rest+=1;
		}
	}
	return(node);
}

//function #11
/*This function helps to list each available restaurant with corresponding list of prices of required items*/
void get_available_rest(struct TNode** items,struct restaurants* node,struct availableRestaurant* av_rest,int* restId,float* totalPrice,int i,int argc)
{
	int j,k;	
	av_rest->rest_id=node->rest_id[i];
	*restId=node->rest_id[i];
	*totalPrice=0;		
	for(j=0;j<(argc-2);j++)
	{
		for(k=0;k<items[j]->no_rest;k++)
		{
			if(items[j]->rest_id[k]==av_rest->rest_id)
			{
				av_rest->tag_no[j]=items[j]->tag_no[k];
				av_rest->price[j]=items[j]->price[k];
				break;
			}
		 			
		}
	 
	}
	for(j=0;j<(argc-2);j++)
	{
		for(k=0;k<j;k++) if(av_rest->tag_no[j]==av_rest->tag_no[k]) break;
		if(j==k) (*totalPrice)+=av_rest->price[j];
	}			
}

//functions declared ends here

/*Main function starts*/

int main(int argc, char**argv)
{
	if (argc<3)
	{
		printf("Nil\n");
		return(1); 
	}	
	int i=0,flag=0;
	/*head points to the first node in the linked list*/	
	head=createLinkedList(argv[1]);	
	/*sorting the linked list*/	
	MergeSort(&head);
	struct TNode* item_required[argc-2];
	/* Getting the root of binary tree*/
	struct TNode *root = sortedListToBST(head);
	/*searching in tree for the required items that are parsed from command line*/	
	for(i=0;i<(argc-2);i++)
	 {	
		item_required[i]=doBinarySearch(root,argv[i+2]);
		if(item_required[i]==NULL)
		 {
			flag=1; break;
		 } 
	
	 }
	 if(flag==1) 
	  {
		printf("Nil\n");
		return;
	  }
	struct restaurants* rests = (struct restaurants*)malloc(sizeof(struct restaurants));
	/*rests points to the structure of available restaurant Id's*/	
	rests=availableRestaurants(item_required,argc);
	if(rests->no_rest==0)
	{ 
		printf("Nil\n");return(3);
	}
	struct availableRestaurant* rest=(struct availableRestaurant*)malloc((rests->no_rest)*sizeof(struct availableRestaurant));
	/* array 'a' stores all the available restaurant Id's*/	
	int a[rests->no_rest];
	/* array*/
	float b[rests->no_rest];
	/* array 'b' stores the total costs for required items in the available restaurants*/
	for(i=0;i<rests->no_rest;i++)
	{
		get_available_rest(item_required,rests,rest,&a[i],&b[i],i,argc);
		rest++;
	}
	rest-=rests->no_rest;
	/* finding out the available restaurant which has the least total cost for the specified items*/	
	for(i=1;i<rests->no_rest;i++)
	{
		if(b[i-1]<b[i]) 
		{
			b[i]=b[i-1];
			a[i]=a[i-1];
		}
	}
	/* printing out the restaurant Id and total price corresponding to the available restaurant which costs less  */
	printf("%d, %0.2f\n",a[i-1],b[i-1]);
}

 


 


