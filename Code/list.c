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

int printMyList(int i, void* env){
	fprintf((FILE*)env, "%d ", i);
	return i;
}

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
	//printf("remove %i  at position %i\n", node_to_remove->value, pos-1);
	//printf(" New List : ");
	//list_map(l, printMyList, stdout);
	//printf("\n");
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
	(void)f;
	return l;
}

