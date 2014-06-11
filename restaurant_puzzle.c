# include "stdio.h"
#include "string.h"
#include "stdlib.h"
         
/* structures  */

//structure #1
/* NODE will store available restaurants and prices over there for an item */
struct NODE 
{
	char word[50];
	int initial_rest_id;
	int initial_tag_no;
	float initial_price;
	int* rest_id;
	int* tag_no;
	float* price;
	int no_rest;
	struct NODE *next;
};

//structure #2
/* TNode is actually tree node corresponding to a NODE */
struct TNode
{
	char word[50];
	int* rest_id;
	float* price;
	int* tag_no;
	int no_rest;
        struct TNode* left;
        struct TNode* right;
};

//structure #3
/* restaurants stores available restaurant ID's for the required items */
struct restaurants
{ 	int no_indep_rest;
	int* rest_id;
	int* no_tags;
	
};

////structure #4
/* availableRestaurant stores a available restaurant ID and the prices of required items over there along with it's tag number */
struct availableRestaurant
{ 
	int rest_id;
	int* tag_no;
	int* no_of_a_specific_item;
	float* price;

};

/* functions */

//function #1
/* this function reads data from a csv file and write into a linked list, also find out the total number of rows and different resturants in the csv file  */
void createLinkedList(char *a,struct NODE** head, int* total_tags)
{	
	FILE *data;
	data = fopen(a, "r");
	char parsedLine[500];
	int i=0,n=0;
	*total_tags=1;		
	while (fgets(parsedLine, 500, data) != NULL)
	{
		if(parsedLine[0]!='\n')
		{	    	
			struct NODE *node = malloc(sizeof(struct NODE));
		    	char *getRest_id = strtok(parsedLine, ", ");
		    	node->initial_rest_id = atoi(getRest_id);
		    	char *getPrice = strtok(NULL, ", "); 
		    	node->initial_price = atof(getPrice);
		    	char *getWord = strtok(NULL, ", ");
		    	strcpy(node->word, getWord);
			n=strlen(node->word);
			if(node->word[n-1] =='\n') { node->word[n-2]='\0';}
		    	node->initial_tag_no=(*total_tags);
			node->no_rest=1;
		    	node->next = *head;
		    	*head = node; 

		    	while( (getWord = strtok(NULL, ", ") ) != NULL)
			{
				if(getWord[0]!='\n')		    	
				{	
					struct NODE *node =(struct NODE * )malloc(sizeof(struct NODE));
			    		strcpy(node->word, getWord);
					n=strlen(node->word);
					if(node->word[n-1] =='\n') { node->word[n-2]='\0';}
			   		node->initial_rest_id = atoi(getRest_id);
			    		node->initial_price = atof(getPrice);
			    		node->initial_tag_no=*total_tags;
					node->no_rest=1;
			    		node->next = *head;
			    		*head = node; 
				}
			}
		}	
	(*total_tags)++;
	   
	}
	fclose(data);
	
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
	node->rest_id=(int* )malloc((any_node->no_rest)*sizeof(int));
	node->tag_no=(int* )malloc((any_node->no_rest)*sizeof(int));
	node->price=(float* )malloc((any_node->no_rest)*sizeof(float));
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
struct restaurants* availableRestaurants(struct TNode** items,int argc,int total_tags)
{       	
	struct restaurants* node = (struct restaurants*)malloc(sizeof(struct restaurants));
	node->no_indep_rest=0;
	node->rest_id=(int* )malloc(total_tags*sizeof(int));
	int i=0,j=0,k=0,m=0;
	int searchTherestIdInTheItem(struct TNode* node,int rest_id)
	{	
		int i=0,j=0;
		for(i=0;i<node->no_rest;i++)
		{
			 if(node->rest_id[i]==rest_id) 
			 {
				j=1;
			 }
		}
	        return(j);
	}
	for(j=0;j<items[0]->no_rest;j++)
	{	
		for(m=0;m<j;m++) if(items[0]->rest_id[m]==items[0]->rest_id[j]) break;
		if(m==j)
		{
			for(i=1;i<(argc-2);i++)
			{
				k=searchTherestIdInTheItem(items[i],items[0]->rest_id[j]);
				if(k==0) break;
			}
			if(i==argc-2) 
			{
				node->rest_id[node->no_indep_rest]=items[0]->rest_id[j];			
				node->no_indep_rest+=1;
			}
		}
	}
	int count_rest_id(struct TNode** items,int rest_id)
	{
		int j,k,count=0;
		for(j=0;j<(argc-2);j++)
		{
			for(k=0;k<items[j]->no_rest;k++)
			{
				if(items[j]->rest_id[k]==rest_id)
				{
					count++;
				}
			 			
			}
	 	}
		return(count);
	}
	node->no_tags=(int* )malloc((node->no_indep_rest)*sizeof(int));
	for(i=0;i<(node->no_indep_rest);i++) (node->no_tags[i])=count_rest_id(items,node->rest_id[i]);
	return(node);
}

//function #11
/*This function helps to list each available restaurant with corresponding list of prices of required items*/
void get_available_rest(struct TNode** items,struct restaurants* node,struct availableRestaurant* av_rest,int* restId,float* totalPrice,int i,int argc)
{
	int j,k,x=0,a[argc-2],b[argc-2],total_combinations=1;
	float min=1000;
	av_rest->tag_no = (int* )malloc((node->no_tags[i])*sizeof(int));
	av_rest->price = (float* )malloc((node->no_tags[i])*sizeof(float));
	av_rest->no_of_a_specific_item = (int* )malloc((argc-2)*sizeof(int));
	av_rest->rest_id=node->rest_id[i];
	*restId=node->rest_id[i];
	*totalPrice=0;		
	for(j=0;j<(argc-2);j++)
	{
		av_rest->no_of_a_specific_item[j]=0;
		for(k=0;k<items[j]->no_rest;k++)
		{
			if(items[j]->rest_id[k]==av_rest->rest_id)
			{
				av_rest->tag_no[x]=items[j]->tag_no[k];
				av_rest->price[x]=items[j]->price[k];
				av_rest->no_of_a_specific_item[j]++;				
				x++;
			}
		 			
		}
	}	
	a[0]=0;
	b[0]=0;	
	for(j=1;j<(argc-2);j++)
	{
		 b[j]=b[j-1]+av_rest->no_of_a_specific_item[j-1];
		 a[j]=0;
	}
	int increment(int* a,int argc, int* counts)
	{
		for(j=argc-3;j>=0;j--)
		{
			if(++a[j] == counts[j]) a[j]=0;
			else break;
		}
		if(j==-1) return(0);// finished 
		else return(1);// continue
	}
	a:
	for(j=0;j<(argc-2);j++)
	{
		for(k=0;k<j;k++) 
		if((av_rest->tag_no[(a[k]+b[k])]) == (av_rest->tag_no[(a[j]+b[j])])) break;
		if(k==j) (*totalPrice)+=av_rest->price[(a[j]+b[j])];
			
	}	
					
	if((*totalPrice)<min) min=*totalPrice;	
	if(increment(a,argc,av_rest->no_of_a_specific_item)==1)
	{
		*totalPrice=0;
		 goto a;
	}	
	*totalPrice=min;	
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
	
	int i=0,flag=0,n=0,rest_id=0,minRest_id=0;
	float price=0,minPrice=1000;
	/*head wants to point to the first node in the linked list*/	
	struct NODE* head=NULL;
	/*items in a row of the csv file gets a tag number and total tags is aslo the number of rows in the file*/
	int total_tags=0;
	/* linked list is being created here */
	createLinkedList(argv[1],&head,&total_tags);
	/*dummy helps to traverse the linked list and do necessory memory alocations for the members of each nodes in the linked list */
	struct NODE* dummy = (struct NODE* )malloc(sizeof(struct NODE));
	dummy=head;	
	while(dummy!=NULL)
	{
		dummy->rest_id=(int* )malloc(total_tags*sizeof(int));
		dummy->tag_no=(int* )malloc(total_tags*sizeof(int));
		dummy->price=(float* )malloc(total_tags*sizeof(float));
		dummy->rest_id[0]=dummy->initial_rest_id;
		dummy->tag_no[0]=dummy->initial_tag_no;
		dummy->price[0]=dummy->initial_price;
		dummy=dummy->next;
	}
	/*sorting the linked list*/	
	MergeSort(&head);
	//displayList(head);	
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
	rests=availableRestaurants(item_required,argc,total_tags);
	if(rests->no_indep_rest==0)
	{ 
		printf("Nil\n");return(3);
	}
	struct availableRestaurant* rest=(struct availableRestaurant*)malloc((rests->no_indep_rest)*sizeof(struct availableRestaurant));
	for(i=0;i<rests->no_indep_rest;i++)
	{
		get_available_rest(item_required,rests,rest,&rest_id,&price,i,argc);
		rest++;
		if(price<minPrice) minPrice=price; minRest_id=rest_id;
	}
	rest-=rests->no_indep_rest;	
	/* printing out the restaurant Id and total price corresponding to the available restaurant which costs less  */
	printf("%d, %0.2f\n",minRest_id,minPrice);	
}

 


 


