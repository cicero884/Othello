#include"list.h"


/* sorts the linked list by changing next pointers (not data) */
void Merge_Sort(node** headRef,bool (*compare)(void*,void*)){
	node* head = *headRef;
	node* a;
	node* b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->next == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	FrontBackSplit(head, &a, &b);

	/* Recursively sort the sublists */
	Merge_Sort(&a,compare);
	Merge_Sort(&b,compare);

	/* answer = merge the two sorted lists together */
	*headRef = SortedMerge(a, b,compare);
}
void MoveNode(node** destRef,node** sourceRef) { 
    /* the front source node  */
    node* newNode = *sourceRef; 
    /* Advance the source pointer */
    *sourceRef = newNode->next; 
    /* Link the old dest off the new node */
    newNode->next = *destRef; 
    /* Move dest to point to the new node */
    *destRef = newNode; 
}
/* See https:// www.geeksforgeeks.org/?p=3622 for details of this
   function */
node* SortedMerge(node* a, node* b,bool (*compare)(void*,void*)){
	node* result = NULL;

  /* point to the last result pointer */
	node** lastPtrRef = &result;
	while(1)
	{
		if (a == NULL)
		{
			*lastPtrRef = b;
			break;
		}
		else if (b==NULL)
		{
			*lastPtrRef = a;
			break;
		}
		if(compare(a->data,b->data))
		{
			MoveNode(lastPtrRef, &a);
		}
		else
		{
			MoveNode(lastPtrRef, &b);
		}

		/* tricky: advance to point to the next ".next" field */
		lastPtrRef = &((*lastPtrRef)->next);
	}
	return(result);
}
node* SortedCopyMerge(node* a, node* b,bool (*compare)(void*,void*)){
	node* result = NULL;

  /* point to the last result pointer */
	node** lastPtrRef = &result;
	while(1)
	{
		if (a == NULL)
		{
			while(b){
				node* newNode = malloc(sizeof(node));
				newNode->data=b->data;
				b=b->next;
				newNode->next = *lastPtrRef;
				*lastPtrRef = newNode;
			}
			break;
		}
		else if (b==NULL)
		{
			*lastPtrRef = a;
			break;
		}
		if(compare(a->data,b->data))
		{
			MoveNode(lastPtrRef, &a);
		}
		else
		{
			/* the front source node  */
			node* newNode = malloc(sizeof(node)); 
			newNode->data=b->data;
			/* Advance the source pointer */
			b=b->next;
			/* Link the old dest off the new node */
			newNode->next = *lastPtrRef; 
			/* Move dest to point to the new node */
			*lastPtrRef = newNode; 
			//MoveNode(lastPtrRef, &b);
		}

		/* tricky: advance to point to the next ".next" field */
		lastPtrRef = &((*lastPtrRef)->next);
	}
	return(result);
}

/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back halves,
   and return the two lists using the reference parameters.
   If the length is odd, the extra node should go in the front list.
   Uses the fast/slow pointer strategy. */
void FrontBackSplit(node* source,node** frontRef, node** backRef){
	node* fast;
	node* slow;
	slow = source;
	fast = source->next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL){
		fast = fast->next;
		if (fast != NULL){
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

/* Function to insert a node at the beginging of the linked list */
void Push_List(node** head_ref,void* new_data){
	node* new_node=malloc(sizeof(node));
	new_node->data=new_data;
	new_node->next=(*head_ref);
	(*head_ref)=new_node;
}

void *Pop_List(node** head_ref){
	node *tmp=(*head_ref);
	void *data=tmp->data;
	(*head_ref)=tmp->next;
	free(tmp);
	return data;
}

