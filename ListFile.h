#ifndef LISTFILE_H
#define LISTFILE_H

#include"Node.h"

struct ListFile_t
{
	struct Node_t* head;
	size_t size;
};

void constructList(struct ListFile_t* it);
void copyList(struct ListFile_t* it, struct ListFile_t* src);
void cloneList(struct ListFile_t* it, struct ListFile_t* src);
void destroyList(struct ListFile_t* it);

int readFile(struct ListFile_t* it, const char* filename);
size_t getSize(struct ListFile_t* it);

int find(struct ListFile_t* it, const char* name);
int removeByName(struct ListFile_t* it, const char* name);
int insert(struct ListFile_t* it, const char* name, void* data, size_t size);
ssize_t appendFromFile(struct ListFile_t* it, const char* filename);
int saveToFile(struct ListFile_t* it, const char* filename);
size_t getElementSize(struct ListFile_t* it, size_t index);
void* getElementData(struct ListFile_t* it, size_t index);
const char* getElementName(struct ListFile_t* it, size_t index);

#endif
