#ifndef STRLIST_H
#define STRLIST_H

#include <malloc.h>

typedef struct strnode
{
	char *str;
	struct strnode *next;
} StrNode;

typedef struct strlist
{
	struct strnode *first;
	struct strnode *last;
} StrList;

StrList *strlist_new();
void strlist_insert(StrList *list, char *str);
void strlist_delete(StrList *list);

#endif //STRLIST_H
