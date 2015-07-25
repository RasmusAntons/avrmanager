#include "strlist.h"

StrList *strlist_new()
{
	StrList *list = malloc(sizeof(StrList));
	list->first = NULL;
	return list;
}

int strlist_is_empty(StrList *list)
{
	return list->first == NULL;
}

void strlist_insert(StrList *list, char *str)
{
	if (strlist_is_empty(list))
	{
		list->first = malloc(sizeof(StrNode));
		list->first->next = NULL;
		list->last = list->first;
		list->iterator = list->first;
		list->first->str = str;
	}
	else
	{
		list->last->next = malloc(sizeof(StrNode));
		list->last = list->last->next;
		list->last->next = NULL;
		list->last->str = str;
	}
}

void strlist_reset_iterator(StrList *list)
{
	list->iterator = list->first;
}

int strlist_has_next(StrList *list)
{
	return list->iterator != NULL;
}

char *strlist_get_next(StrList *list)
{
	if (list->iterator == NULL)
	{
		return NULL;
	}
	else
	{
		char *str = list->iterator->str;
		list->iterator = list->iterator->next;
		return str;
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
