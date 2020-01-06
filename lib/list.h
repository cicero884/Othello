#ifndef LIST_H
#define LIST_H

#include<stdbool.h>
#include<stdlib.h>

typedef struct Node{
	void* data;
	struct Node* next;
} node;

/* function prototypes */
void Merge_Sort(node** headRef,bool (*compare)(void*,void*));
	node* SortedMerge(node* a, node* b,bool (*compare)(void*,void*));
	void FrontBackSplit(node* source,node** frontRef, node** backRef);
node* SortedCopyMerge(node* a, node* b,bool (*compare)(void*,void*));
void Push_List(node** head_ref,void* new_data);
void *Pop_List(node** head_ref);
#define Drop_List(x) while(*(x)) Pop_List(x)

#endif
