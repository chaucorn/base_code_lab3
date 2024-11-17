/*-----------------------------------------------------------------*/
/*
 Licence Informatique - Structures de données
 Mathias Paulin (Mathias.Paulin@irit.fr)
 
 Implantation du TAD List vu en cours.
 */
/*-----------------------------------------------------------------*/
#include <stdio.h>
#
#include <stdlib.h>
#include <assert.h>

#include "list.h"


SubList list_mergesort(SubList l, OrderFunctor f);

typedef struct s_LinkedElement {
	int value;
	struct s_LinkedElement* previous;
	struct s_LinkedElement* next;
} LinkedElement;

/* Use of a sentinel for implementing the list :
 The sentinel is a LinkedElement* whose next pointer refer always to the head of the list and previous pointer to the tail of the list
 */
struct s_List {
	LinkedElement* sentinel;
	int size;
};

/*Struct SubList minimal representation of a linked list*/
struct s_SubList {
	LinkedElement* list_head;
	LinkedElement* list_tail;
}; 

/*-----------------------------------------------------------------*/

List* list_create(void) {
	List* l;
	l = malloc(sizeof(struct s_List)+sizeof(LinkedElement));
	l->sentinel = (LinkedElement*) (l + 1);
	l->size = 0;
	(l->sentinel)->next = l->sentinel;
	(l->sentinel)->previous = l->sentinel;
 	return l;
}

/*-----------------------------------------------------------------*/

List* list_push_back(List* l, int v) {
	LinkedElement* my_sentinel = l->sentinel;
	LinkedElement* new_tail = malloc(sizeof(LinkedElement));
	new_tail ->value = v;
	if ((my_sentinel->next == my_sentinel)) // the list is previously empty
	{
		my_sentinel->next = new_tail;
		my_sentinel ->previous = new_tail;
		new_tail->next = my_sentinel;
		new_tail->previous = my_sentinel;
	}else{
		LinkedElement* list_tail = my_sentinel->previous;
		// point list_tail to new tail:
		list_tail->next = new_tail;
		// pointing the new_tail
		new_tail->previous = list_tail;
		new_tail->next = my_sentinel;
		my_sentinel->previous = new_tail;
	}
	l->size+=1;
	return l;
}

/*-----------------------------------------------------------------*/

void list_delete(ptrList* l) {
	LinkedElement* sentinel = (*l)->sentinel;
	LinkedElement* toDelete = sentinel->next;
	while (toDelete !=  sentinel)
	{
		LinkedElement* f = toDelete;
		toDelete = toDelete->next;
		free(f);
	}
	free(*l);

}

/*-----------------------------------------------------------------*/

List* list_push_front(List* l, int v) {
	LinkedElement* sentinel = l->sentinel;
	LinkedElement* new_head = malloc(sizeof(LinkedElement));
	new_head->value = v;
	if (sentinel->next == sentinel) // list is empty
	{
		sentinel->next = new_head;
		sentinel ->previous = new_head;
		new_head->next = sentinel;
		new_head->previous = sentinel;
	}else{
		LinkedElement* list_head = sentinel->next;
		sentinel->next = new_head;
		list_head->previous = new_head;
		new_head->next = list_head;
		new_head->previous = sentinel;
	}
	l->size+=1;
	return l;
	
}

/*-----------------------------------------------------------------*/

int list_front(const List* l) {
	LinkedElement* sentinel = l->sentinel;
	return (sentinel->next)->value;
}

/*-----------------------------------------------------------------*/

int list_back(const List* l) {
	LinkedElement* sentinel = l->sentinel;
	return (sentinel->previous)->value;
}

/*-----------------------------------------------------------------*/

List* list_pop_front(List* l) {
	LinkedElement* sentinel = l->sentinel;
	LinkedElement*old_head = sentinel->next;
	LinkedElement*new_head = old_head->next;
	sentinel->next = new_head;
	new_head->previous = sentinel;
	free(old_head);
	return l;
}

/*-----------------------------------------------------------------*/

List* list_pop_back(List* l){
	LinkedElement* sentinel = l->sentinel;
	LinkedElement*old_tail = sentinel->previous;
	LinkedElement*new_tail = old_tail->previous;
	sentinel->previous = new_tail;
	new_tail->next = sentinel;
	free(old_tail);
	return l;
}

/*-----------------------------------------------------------------*/

List* list_insert_at(List* l, int p, int v) {
	LinkedElement* new_node = malloc(sizeof(LinkedElement));
	new_node->value = v;
	LinkedElement*sentinel = l->sentinel;
	LinkedElement*current_node = sentinel;
	int pos = 0;
	while(pos < p){
		current_node = current_node->next;
		pos++;
	}
	// rewire the nodes
	LinkedElement* next_node = current_node->next;
	current_node->next = new_node;
	next_node->previous = new_node;
	new_node->previous = current_node;
	new_node->next =next_node;
	// increase the size
	l->size += 1;
	return l;

}

/*-----------------------------------------------------------------*/


List* list_remove_at(List* l, int p) {
	LinkedElement*sentinel = l->sentinel;
	LinkedElement*node_to_remove = sentinel;
	int pos = 0;
	while(pos <= p){
		node_to_remove = node_to_remove->next;
		pos++;
	}
	// rewire the nodes
	LinkedElement* prev_node_to_remove = node_to_remove->previous;
	LinkedElement* next_node_to_remove = node_to_remove->next;
	prev_node_to_remove->next = next_node_to_remove;
	next_node_to_remove->previous = prev_node_to_remove;
	// increase the size
	l ->size -=1;
	free(node_to_remove); 
	return l;
}

/*-----------------------------------------------------------------*/

int list_at(const List* l, int p) {
	LinkedElement* current_node = l->sentinel->next;
	int pos = 0;
	while(pos < p){
		current_node = current_node->next;
		pos++;
	}
	return current_node->value;
}

/*-----------------------------------------------------------------*/

bool list_is_empty(const List* l) {
	return(l->size==0);
}

/*-----------------------------------------------------------------*/

int list_size(const List* l) {
	return l->size;
}

/*-----------------------------------------------------------------*/

List* list_map(List* l, ListFunctor f, void* environment) {
	LinkedElement* sentinel = l->sentinel;
	for (LinkedElement* element = sentinel->next; element != sentinel; element = element->next){
		f(element->value, environment);
	} 
	return l;
}


/*-----------------------------------------------------------------*/

List* list_sort(List* l, OrderFunctor f) {
	LinkedElement* sentinel = l->sentinel;
	SubList my_sublist;
	my_sublist.list_head = sentinel->next;
	my_sublist.list_tail = sentinel->previous;

	//cut the link with sentinel
	sentinel->next->previous = NULL;
	sentinel->previous->next = NULL;
	sentinel->next = NULL;
	sentinel->previous = NULL;
	my_sublist = list_mergesort(my_sublist, f);
	printf("received my sorted list\n");
	sentinel->next = my_sublist.list_head;
	(sentinel->next)->previous = sentinel;
	sentinel->previous = my_sublist.list_tail;
	(sentinel->previous)->next = sentinel;
	printf("finished sorting the list!\n");
	return l;
}

/*-----------------------------------------------------------------*/

SubList list_split(SubList l){
	SubList splitted_list;
	LinkedElement* slow_pointer = l.list_head;
	LinkedElement* fast_pointer = l.list_head;

	if (l.list_head == l.list_tail)
	{
		return splitted_list;
	}

	while(fast_pointer != l.list_tail && fast_pointer->next != l.list_tail){
		slow_pointer = slow_pointer->next;
		fast_pointer = fast_pointer->next->next;
	}
	
	splitted_list.list_head = slow_pointer;
	splitted_list.list_tail =slow_pointer->next;
	slow_pointer->next = NULL;
	return splitted_list;
}

SubList list_merge(SubList leftlist, SubList rightlist, OrderFunctor f){
	SubList merged_list;
	LinkedElement* tail_list_itr = NULL;
	LinkedElement* leftlist_itr = leftlist.list_head;
	LinkedElement* rightlist_itr = rightlist.list_head;
	/*
	printf("BEFORE MERGE:\n");
    if (leftlist.list_head && leftlist.list_tail) {
        printf("leftlist head %i leftlist tail %i\n", leftlist.list_head->value, leftlist.list_tail->value);
    } else {
        printf("leftlist is empty\n");
    }
    if (rightlist.list_head && rightlist.list_tail) {
        printf("rightlist head %i rightlist tail %i\n", rightlist.list_head->value, rightlist.list_tail->value);
    } else {
        printf("rightlist is empty\n");
    }
	*/
	// find the head of merged list:
	if (f(leftlist_itr->value, rightlist_itr->value)){
		merged_list.list_head = leftlist_itr;
		leftlist_itr = leftlist_itr->next;
	}
	else{
		merged_list.list_head = rightlist_itr;
		rightlist_itr = rightlist_itr->next;
	}
	// Point the tail iterator to the head
	tail_list_itr = merged_list.list_head;
	
	// need to make sure that the last element point to NULL 
	while (leftlist_itr!= NULL && rightlist_itr!= NULL){
		if (f(leftlist_itr->value, rightlist_itr->value)){
			//bind the tail and its next element
			tail_list_itr->next = leftlist_itr;
			leftlist_itr->previous = tail_list_itr;
			//update the list tail
			tail_list_itr = leftlist_itr;
			// update, point the element to its next element
			leftlist_itr = leftlist_itr->next;
		}else{
			tail_list_itr->next = rightlist_itr;
			rightlist_itr->previous = tail_list_itr;
			tail_list_itr = rightlist_itr;
			// update, point the element to its next element
			rightlist_itr = rightlist_itr->next;
		}
	} 
	tail_list_itr->next = (leftlist_itr!= NULL)?leftlist_itr:rightlist_itr;
	tail_list_itr = tail_list_itr->next;
	while (tail_list_itr->next)
	{
		tail_list_itr= tail_list_itr->next;
	}
	
	merged_list.list_tail = tail_list_itr;
	/*printf("AFTER MERGE\n");
	printf("merged list head %i merged list tail %i\n", merged_list.list_head->value, merged_list.list_tail->value);
	printf("my list is: ");
	LinkedElement* element = merged_list.list_head;
	while (element != NULL)
	{
		printf(" %i ", element->value);
		
		element = element->next;
	}

	printf("\nfinish printing\n");
	*/
	return merged_list;
}

SubList list_mergesort(SubList l, OrderFunctor f){
	if (l.list_head == l.list_tail){
		return l;
	}else{
		// split the list into 2 sublist
		SubList splitted_list = list_split(l);
		SubList rightlist;
		SubList leftlist;
		leftlist.list_head = l.list_head;
		leftlist.list_tail = splitted_list.list_head;
		rightlist.list_head = splitted_list.list_tail;
		rightlist.list_tail = l.list_tail; 

		// 2 recursive calls
		leftlist = list_mergesort(leftlist, f);
		//printf("leftlist head %i leftlist tail %i\n", leftlist.list_head->value,leftlist.list_tail->value );
		rightlist = list_mergesort(rightlist, f);
		//printf("rightlist head %i rightlist tail %i\n", rightlist.list_head->value,rightlist.list_tail->value );
		return list_merge(leftlist, rightlist, f);
	}
}


	