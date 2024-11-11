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
	free(sentinel); // free the sentinel struct itself ??
	free(*l);
	*l = NULL;
}

/*-----------------------------------------------------------------*/

List* list_push_front(List* l, int v) {
	(void)v;
	return l;
}

/*-----------------------------------------------------------------*/

int list_front(const List* l) {
	(void)l;
	return 0;
}

/*-----------------------------------------------------------------*/

int list_back(const List* l) {
	(void)l;
	return 0;
}

/*-----------------------------------------------------------------*/

List* list_pop_front(List* l) {
	return l;
}

/*-----------------------------------------------------------------*/

List* list_pop_back(List* l){
	return l;
}

/*-----------------------------------------------------------------*/

List* list_insert_at(List* l, int p, int v) {
	(void)v;
	(void)p;
	return l;
}

/*-----------------------------------------------------------------*/

List* list_remove_at(List* l, int p) {
	(void)p;
	return l;
}

/*-----------------------------------------------------------------*/

int list_at(const List* l, int p) {
	(void)l;
	return p;
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
	(void)f;
	return l;
}

