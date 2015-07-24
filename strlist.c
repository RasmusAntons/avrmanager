#include "strlist.h"

StrList *strlist_new()
{
	StrList *newlist = malloc(sizeof(StrList));
	newlist->first = malloc(sizeof(StrNode));
	newlist->first->str = NULL;
	newlist->first->next = malloc(sizeof(StrNode));
	newlist->last = newlist->first->next;
	newlist->last->str = NULL;
	newlist->last->next = NULL;
	return newlist;
}

void strlist_insert(StrList *list, char *str)
{
	if (strlist_is_empty(list))
	{
		list->first->str = str;
	}
	else
	{
		list->last->str = str;
		list->last->next = malloc(sizeof(StrNode));
		list->last = list->last->next;
	}
}

void strlist_delete(StrList *list)
{
	StrNode *node;
	StrNode *next;
	node = list->first;
	
	while(node->next != NULL)
	{
		next = node->next;
		free(node);
		node = next;
	}
	free(node);
	free(list);
}

int strlist_is_empty(StrList *list)
{
	return list->first->str == NULL;
}
