/*
	CSE 109: Spring 2018
	Dylan Spector
	drs320
	C file for a Node object in a singly liked list
	Program #5
*/

#include<stdlib.h>
#include"Node.h"
#include<string.h>

void constructNode(struct Node_t* it, const char* name, void* data, size_t size, struct Node_t* next)
{
	// Set Name
	if(name)
	{
		it->name = (char*)malloc(strlen(name)+1*sizeof(char));
		strcpy(it->name, name);
	}
	else
	{
		it->name = '\0';
	}

	// Set Data
	if(data)
	{
		it->data = (void*)malloc(size*sizeof(void));
		memcpy(it->data, data, size);
		it->size = size;
	}
	else
	{
		it->data = NULL;
		it->size = 0;
	}

	it->next = next;
}

// Does not free the actual Node_t object. The User will do this.
void destroyNode(struct Node_t* it)
{
	free(it->data);
	free(it->name);
}

const char* getName(struct Node_t* it)
{
	return (const char*)it->name;
}

void* getData(struct Node_t* it)
{
	return it->data;
}

size_t getNodeSize(struct Node_t* it)
{
	return it->size;
}

struct Node_t* getNext(struct Node_t* it)
{
	return it->next;
}

void setName(struct Node_t* it, char* name)
{
    free(it->name);
	if(name)
    {
        it->name = (char*)malloc(strlen(name)+1*sizeof(char));
        strcpy(it->name, name);
    }
    else
    {
        it->name = '\0';
    }
}

void setData(struct Node_t* it, void* data, size_t size)
{
	free(it->data);
	if(data)
    {
        it->data = (void*)malloc(size*sizeof(void));
        memcpy(it->data, data, size);
        it->size = size;
    }
    else
    {
        it->data = NULL;
        it->size = 0;
    }
}

void setNext(struct Node_t* it, struct Node_t* next)
{
	it->next = next;
}
