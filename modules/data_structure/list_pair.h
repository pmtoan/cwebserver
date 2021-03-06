#ifndef __DATA_STRUCTURE_LIST_PAIR_H__
#define __DATA_STRUCTURE_LIST_PAIR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../core/types.h"
#include "../utilities/utils_string.h"

/* --- pair data structure --- */
typedef struct PAIR
{
    char* key;
    char* value;
    struct PAIR* next;
}PAIR;

typedef struct LIST_PAIR
{
    /*
    *  @LIST_PAIR a single link list of PAIR
    */
    int len;
    PAIR* head;
}LIST_PAIR;

/*----------------------------------------------------------------*/
LIST_PAIR list_pair_init();
void list_pair_free(LIST_PAIR* list);
void list_pair_print(LIST_PAIR list);
void list_pair_append(LIST_PAIR* list, const char* key, const char* value);
char* list_pair_find(LIST_PAIR list, const char* key);
LIST_PAIR list_pair_parse_x_www_form_urlencoded(const char* msg);
char* list_pair_compose_x(LIST_PAIR list);
char* list_pair_compose(LIST_PAIR list);
/*----------------------------------------------------------------*/


LIST_PAIR list_pair_init()
{
	/*
	 *	todo @list_pair_init() initialize a list of pair holder
	 *	usage LIST_PAIR list = list_pair_init()
	*/
	LIST_PAIR list;
	list.len = 0;
	list.head = NULL;
	return list;
}

void list_pair_free(LIST_PAIR* list)
{
	/*
	 *	todo @list_pair_free release memory of @list
	 *	usage
	 *		LIST_PAIR list = list_pair_init()
	 *		...
	 *		list_pair_free(&list)
	*/
	while(list->head != NULL)
	{
		PAIR* i = list->head;
		list->head = list->head->next;
		free(i->key);
		free(i->value);
		i->next = NULL;
		free(i);
	}
}

void list_pair_print(LIST_PAIR list)
{
	/*
	*	todo @list_pair_print print @list
	*/
	int i = 0;
	while(i < list.len)
	{
		_("%s: %s\n", list.head->key, list.head->value);
		i++;
		list.head = list.head->next;
	}
}

void list_pair_append(LIST_PAIR* list, const char* key, const char* value)
{
	/*
	 *	todo @list_pair_append append a @key-@value pair to @list
	 *	usage
	 *		LIST_PAIR list = list_pair_init()
	 *		list_pair_append(&list, "name", "list_pair");
	 *		...
	 *		list_pair_free(&list)
	*/
	if (list->len == 0)
	{
		list->head = (PAIR*)malloc(sizeof(PAIR*));
		list->head->key = strdup(key);
		list->head->value = strdup(value);
		list->head->next = NULL;
	}
	else
	{
		PAIR* i = list->head;
		while(i->next != NULL)
			i = i->next;
		PAIR* next = (PAIR*)malloc(sizeof(PAIR*));
		next->key = strdup(key);
		next->value = strdup(value);
		next->next = NULL;
		i->next = next;
	}
	list->len++;
}

char* list_pair_find(LIST_PAIR list, const char* key)
{
	/*
	 *	todo @list_pair_find search and get value contain @key from @list
	 *	Return "" if key is not found, otherwise, return value of pair
	 *	usage
	 *		LIST_PAIR list = list_pair_init()
	 *		list_pair_append(&list, "name", "list_pair");
	 *		...
	 *		list_pair_find(list, "name"); --> return "list_pair"
	 *		list_pair_find(list, "msg"); --> return NULL
	 *		list_pair_free(&list)
	*/

	while(list.head != NULL)
	{
		if (strcmp(list.head->key, key) == 0)
			return list.head->value;
		list.head = list.head->next;
	}
	return "";
}

LIST_PAIR list_pair_parse_x_www_form_urlencoded(const char* msg)
{
	/*
	 *	todo @list_pair_parse_x_www_form_urlencoded parse a string in x-www-form-urlencoded format
	 *	x-www-form-urlencoded format: name=toan&school=university of information technology
	 *	usage
	 *		LIST_PAIR list = list_pair_parse_x_www_form_urlencoded("name=toan&school=university of information technology")
	 *		list = {
	 *			"name": "toan",
	 *			"school": "university of information technology"
	 *		}
	*/
	LIST_PAIR list = list_pair_init();
	int estimate_length = string_count_cha(msg, '&');
	char** pairs = string_split(msg, "&", estimate_length);
	for (int i = 0; i <= estimate_length; ++i)
	{
		char** pair = string_split(pairs[i], "=", 1);
		list_pair_append(&list, string_to_ascii(pair[0]), string_to_ascii(pair[1]));
		free(pair[0]);
		free(pair[1]);
		free(pair);
	}
	for (int i = 0; i <= estimate_length; ++i)
		free(pairs[i]);
	free(pairs);
	return list;
}

char* list_pair_compose_x(LIST_PAIR list)
{
	/*
	 *	todo @list_pair_compose_x compose LIST_PAIR to x-www-form-urlencoded string
	 *	x-www-form-urlencoded string: name=toan&school=university of information technology
	 *	usage
	 *		LIST_PAIR list = list_pair_init()
	 *		list_pair_append(&list, "name", "toan");
	 *		list_pair_append(&list, "school", "university of information technology");
	 *		char* str = list_pair_compose_x(list)
	 *		...
	 *		list_pair_free(&list)
	*/
	char* buffer = (char*)malloc(__SIZE_EXTRA__);
	for (int i = 0; i < list.len; ++i)
	{
		if (i==0)
			sprintf(buffer, "%s=%s", list.head->key, list.head->value);
		else
			sprintf(buffer, "%s&%s=%s", buffer, list.head->key, list.head->value);
		list.head = list.head->next;
	}
	char* rt = strdup(buffer);
	free(buffer);
	return rt;
}

char* list_pair_compose(LIST_PAIR list)
{
	/*
	 *	todo @list_pair_compose compose a LIST_PAIR to object string
	 *	object string: {"name":"toan","school":"university of information technology"}
	 *	usage
	 *		LIST_PAIR list = list_pair_init()
	 *		list_pair_append(&list, "name", "toan");
	 *		list_pair_append(&list, "school", "university of information technology");
	 *		char* obj_string = list_pair_compose(list)
	 *		...
	 *		list_pair_free(&list)
	*/
	char* buffer = (char*)malloc(__SIZE_EXTRA__);
	sprintf(buffer, "{");
	for (int i = 0; i < list.len; ++i)
	{
		if (i==0)
			sprintf(buffer, "%s\"%s\":\"%s\"", buffer, list.head->key, list.head->value);
		else
			sprintf(buffer, "%s,\"%s\":\"%s\"", buffer, list.head->key, list.head->value);
		list.head = list.head->next;
	}
	sprintf(buffer, "%s}", buffer);
	char* obj_string = strdup(buffer);
	free(buffer);
	return obj_string;
}

#endif // __DATA_STRUCTURE_LIST_PAIR_H__