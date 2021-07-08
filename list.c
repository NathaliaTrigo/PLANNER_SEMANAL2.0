#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

typedef struct Node Node;

struct Node
{
    void *data;
    Node *next;
    Node *prev;
};

struct List
{
    Node *head;
    Node *tail;
    Node *current;
    size_t size;
};

typedef List List;

Node *createNode(void *data)
{
    Node *new_node = (Node *)calloc(1, sizeof(Node));
    assert(new_node);
    if (!new_node)
    {
        return (NULL);
    }
    new_node->data = data;
    return (new_node);
}

List *createList()
{
    List *new_list = calloc(1, sizeof(List));
    assert(new_list);
    if (!new_list)
    {
        return (NULL);
    }
    return (new_list);
}

void *firstList(List *list)
{
    list->current = list->head;
    if (!list->current)
    {
        return (NULL);
    }
    return ((void *)list->current->data);
}

void *nextList(List *list)
{
    if (!list->current)
    {
        return (NULL);
    }
    list->current = list->current->next;
    if (!list->current)
    {
        return (NULL);
    }
    return ((void *)list->current->data);
}

void *lastList(List *list)
{
    Node *temp = list->tail;
    if (temp)
    {
        list->current = temp;
        return ((void *)temp->data);
    }
    else
    {
        // Tail is NULL, manually search from the current
        Node *it = list->current;
        if (!it)
        {
            it = list->head;
        }
        if (!it)
        {
            return (NULL); /* Empty! */
        }
        while (it->next)
        {
            it = it->next;
        }
        list->tail = it;
        list->current = list->tail;
        return ((void *)it->data);
    }
}

void *prevList(List *list)
{
    if (!list->current)
    {
        return (NULL);
    }
    list->current = list->current->prev;
    if (!list->current)
    {
        return (NULL);
    }
    return ((void *)list->current->data);
}

void pushFront(List *list, void *data)
{
    Node *new_node = createNode(data);
    new_node->next = list->head;
    list->head = new_node;
    if (!list->tail)
    {
        list->tail = new_node;
    }
    ++list->size;
}

void pushBack(List *list, void *data)
{
    list->current = list->tail;
    pushCurrent(list, data);
}

void pushCurrent(List *list, void *data)
{
    Node *temp = list->current;
    Node *next = NULL;
    if (temp)
    {
        next = temp->next;
    }
    else
    {
        return (pushFront(list, data));
    }

    Node *new_node = createNode(data);
    temp->next = new_node;
    new_node->prev = temp;
    new_node->next = next;
    if (next)
    {
        next->prev = new_node;
    }
    else
    {
        list->tail = new_node;
    }
    ++list->size;
}

void *popFront(List *list)
{
    list->current = list->head;
    return popCurrent(list);
}

void *popBack(List *list)
{
    list->current = list->tail;
    return popCurrent(list);
}

void *popCurrent(List *list)
{
    Node *temp = list->current;
    if (!temp)
    {
        return (NULL);
    }

    Node *next = temp->next;
    Node *prev = temp->prev;

    if (temp == list->head)
    {
        list->head = next;
    }
    if (temp == list->tail)
    {
        list->tail = prev;
    }

    if (next)
    {
        next->prev = prev;
    }
    if (prev)
    {
        prev->next = next;
    }

    void *data = temp->data;
    free(temp);
    --list->size;
    return ((void *)data);
}

void cleanList(List *list)
{
    while (list->head != NULL)
    {
        popFront(list);
    }
}

size_t sizeOfList(List *list)
{
    return (list->size);
}

void *getList(List *list, size_t i)
{
	if (i > list->size) { return(NULL); }

	Node* node = list->head;
	for(int c = 0; c < i; ++c)
	{
		node = node->next;
	}
	return(node->data);
}