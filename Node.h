#ifndef NODE_H
#define NODE_H

#include<stdlib.h>

struct Node_t
{
	char* name;
	void* data;
	size_t size;
	struct Node_t* next;
};

void constructNode(struct Node_t*, const char*, void*, size_t, struct Node_t*);
void destroyNode(struct Node_t*);

const char* getName(struct Node_t*);
void* getData(struct Node_t*);
size_t getNodeSize(struct Node_t*);
struct Node_t* getNext(struct Node_t*);

void setName(struct Node_t*, char*);
void setData(struct Node_t*, void*, size_t);
void setNext(struct Node_t*, struct Node_t*);
#endif
