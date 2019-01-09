/*
	CSE 109: Spring 2018
	Dylan Spector
	drs320
	C file used in the creation of a singly linked list
	Program #5
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include"ListFile.h"
#include"Node.h"

void constructList(struct ListFile_t* it)
{
	it->head = NULL;
	it->size = 0;
}

void copyList(struct ListFile_t* it, struct ListFile_t* src)
{
	constructList(it);
	struct Node_t* current = src->head;
	for(size_t i = 0; i < getSize(src); i++)
	{
		insert(it, getName(current), getData(current), getNodeSize(current));
		if(i != getSize(src) - 1)
		{
			current = getNext(current);
		}
	}
}
void cloneList(struct ListFile_t* it, struct ListFile_t* src)
{
	destroyList(it);
	copyList(it, src);
}

// Does not free it. The User does this.
void destroyList(struct ListFile_t* it)
{
	if(!it) // if it points to null
	{
		return;
	}

	struct Node_t* current = it->head;
	struct Node_t* next = getNext(it->head);

	for(size_t i = 0; i < getSize(it); i++)
	{
		destroyNode(current);
		free(current);

		if(i != getSize(it) - 1)
		{
			current = next;
			next = getNext(next);
		}
	}
	return;
}


int readFile(struct ListFile_t* it, const char* filename)
{
	struct ListFile_t* tempList = (struct ListFile_t*)malloc(1 * sizeof(struct ListFile_t));
	constructList(tempList);

	if(appendFromFile(tempList, filename) == -1)
	{
		destroyList(tempList);
		free(tempList);
		return -1;
	}
	else if(appendFromFile(tempList, filename) == 0)
	{
        destroyList(tempList);
        free(tempList);
        return 0;
	}
	else
	{
		destroyList(it);
		constructList(it);
	    struct Node_t* current = tempList->head;
    	for(size_t i = 0; i < getSize(tempList); i++)
    	{
        	insert(it, getName(current), getData(current), getNodeSize(current));
       		if(i != getSize(tempList) - 1)
        	{
            	current = getNext(current);
        	}
    	}

		destroyList(tempList);
		free(tempList);
		return 0;
	}
}

size_t getSize(struct ListFile_t* it)
{
	return it->size;
}

int find(struct ListFile_t* it, const char* name)
{
	struct Node_t* current = it->head;
	for(size_t i = 0; i < getSize(it); i++)
	{
		if(!strcmp(getName(current), name)) // returns !0 if str1 == str2
		{
			return 1;
		}
		else if(i != getSize(it) - 1)
		{
			current = getNext(current);
		}
	}
	return 0;
}

int removeByName(struct ListFile_t* it, const char* name)
{
	struct Node_t* previous = NULL;
    struct Node_t* current = it->head;
    for(size_t i = 0; i < getSize(it); i++)
    {
        if(!strcmp(getName(current), name)) // returns !0 if str1 == str2
        {
			if(!i) // if i == 0, then current == it->head
			{
				it->head = getNext(it->head);
				it->size--;
				destroyNode(current);
				free(current);
				current = NULL; // optional
			}
			else
			{
				setNext(previous, getNext(current));
				it->size--;
                destroyNode(current);
                free(current);
                current = NULL;
			}

            return 1;
        }
        else if(i != getSize(it) - 1)
        {
			previous = current;
            current = getNext(current);
        }
    }
    return 0;
}

int insert(struct ListFile_t* it, const char* name, void* data, size_t size)
{
    struct Node_t* previous = NULL;
	struct Node_t* current = it->head;
    for(size_t i = 0; i < getSize(it); i++)
    {
        if(!strcmp(getName(current), name)) // the same name is found
        {
			return 0;
		}
		else if(strcmp(name, getName(current)) < 0) // the new name goes before the old name
		{
        	if(!i) // if i == 0, then current will be the new head
            {
				struct Node_t* newNode = (struct Node_t*)malloc(1*sizeof(struct Node_t));
				constructNode(newNode, name, data, size, it->head);
				it->head = newNode;
                it->size++;
            }
            else
            {
				struct Node_t* newNode = (struct Node_t*)malloc(1*sizeof(struct Node_t));
                constructNode(newNode, name, data, size, current);
	 	 		setNext(previous, newNode);
                it->size++;
        	}
        	return 1;
        }
        else if (i != getSize(it) - 1)
        {
        	previous = current;
			current = getNext(current);
        }
		else
		{
			struct Node_t* newNode = (struct Node_t*)malloc(1*sizeof(struct Node_t));
            constructNode(newNode, name, data, size, NULL);
            setNext(current, newNode);
            it->size++;
			return 1;
		}
    }
	// If we reach here, the list must be empty.
    struct Node_t* newNode = (struct Node_t*)malloc(1*sizeof(struct Node_t));
    constructNode(newNode, name, data, size, NULL);
    it->head = newNode;
    it->size++;
	return 1;
}

ssize_t appendFromFile(struct ListFile_t* it, const char* filename)
{
	int fd = open(filename, O_RDONLY);
	if(fd == -1)
	{
		fprintf(stderr, "Open in appendFromFile() Failed (%d): %s\n", errno, strerror(errno));
		return -1;
	}

	ssize_t numAdded = 0;
	ssize_t nameLength = 0;
	ssize_t dataLength = 0;
	ssize_t result = 0;

	result = read(fd, &numAdded, 8); // reads 1st 8 bytes, stores value in numAdded
	if(result != 8)
	{
        fprintf(stderr, "Read of Number Of Elements failed (%d): %s\n", errno, strerror(errno));
        close(fd);
        return -1;
	}


	struct ListFile_t* tempList = (struct ListFile_t*)malloc(1 * sizeof(struct ListFile_t));
	constructList(tempList);

	for(ssize_t i = 0; i < numAdded; i++) // Loop through every element(node).
	{
		result = read(fd, &nameLength, 8); // reads 1st 8 bytes of element, stores value in nameLength
    	if(result != 8)
    	{
        	fprintf(stderr, "Read of nameLength failed on loop %ld (%d): %s\n", i, errno, strerror(errno));
        	close(fd);
			destroyList(tempList);
			free(tempList);
        	return -1;
    	}

        result = read(fd, &dataLength, 8); // reads 1st 8 bytes of element, stores value in dataLength
        if(result != 8)
        {
            fprintf(stderr, "Read of dataLength failed on loop %ld (%d): %s\n", i, errno, strerror(errno));
            close(fd);
            destroyList(tempList);
            free(tempList);
            return -1;
        }

		char* tempName = (char*)malloc((nameLength)*sizeof(char)); // tempName is not Null Term.
		result = read(fd, &tempName, nameLength);

		char* name = (char*)malloc((nameLength + 1)*sizeof(char)); // extra 1 for null char
		strncpy(name, tempName, (nameLength)*sizeof(char));

		if(result != nameLength)
        {
            fprintf(stderr, "Read of element name failed on loop %ld (%d): %s\n", i, errno, strerror(errno));
            close(fd);
            destroyList(tempList);
            free(tempList);
            return -1;
        }

        void* data = (void*)malloc(dataLength*sizeof(void));
        result = read(fd, data, dataLength);
        if(result != dataLength)
        {
            fprintf(stderr, "Read of element data failed on loop %ld (%d): %s\n", i, errno, strerror(errno));
            close(fd);
            destroyList(tempList);
            free(tempList);
            return -1;
        }

		insert(tempList, name, data, dataLength);
	}

	struct Node_t* current = tempList->head;
	for(size_t i = 0; i < getSize(tempList); i++)
	{
		insert(it, getName(current), getData(current), getNodeSize(current));
		if(i != getSize(tempList) - 1)
		{
			current = getNext(current);
		}
	}

	close(fd);
    destroyList(tempList);
	free(tempList);
    return numAdded;
}

int saveToFile(struct ListFile_t* it, const char* filename)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if(fd == -1)
    {
        fprintf(stderr, "Open failed in saveToFile() (%d): %s\n", errno, strerror(errno));
        return -1;
    }

    ssize_t numToAdd = getSize(it);
    ssize_t nameLength = 0;
    ssize_t dataLength = 0;
    ssize_t result = 0;

    result = write(fd, &numToAdd, 8); // reads 1st 8 bytes, stores value in numAdded
    if(result != 8)
    {
        fprintf(stderr, "Write of Number Of Elements failed (%d): %s\n", errno, strerror(errno));
        close(fd);
        return -1;
    }

	struct Node_t* current = it->head;
    for(ssize_t i = 0; i < numToAdd; i++) // Loop through every element(node).
    {
		nameLength = strlen(getName(current));
        result = write(fd, &nameLength, 8);
        if(result != 8)
        {
            fprintf(stderr, "Write of nameLength failed on loop %ld (%d): %s\n", i, errno, strerror(errno));
            close(fd);
            return -1;
        }

		dataLength = getNodeSize(current);
        result = write(fd, &dataLength, 8);
        if(result != 8)
        {
            fprintf(stderr, "Write of dataLength failed on loop %ld (%d): %s\n", i, errno, strerror(errno));
            close(fd);
            return -1;
        }

		char* name = (char*)malloc(nameLength*sizeof(char));
		name = (char*)getName(current);
		result = write(fd, &name, nameLength);

		if(result != nameLength)
        {
            fprintf(stderr, "Write of element name failed on loop %ld (%d): %s\n", i, errno, strerror(errno));
            close(fd);
            return -1;
        }

		void* data = (void*)malloc(dataLength*sizeof(void));
        data = getData(current);
        result = write(fd, data, dataLength);

        if(result != dataLength)
        {
            fprintf(stderr, "Write of element data failed on loop %ld (%d): %s\n", i, errno, strerror(errno));
            close(fd);
            return -1;
        }

		if(i != numToAdd - 1)
        {
            current = getNext(current);
        }
    }

	close(fd);
	return 1;
}

size_t getElementSize(struct ListFile_t* it, size_t index)
{
	if(index >= getSize(it))
	{
		fprintf(stderr, "The index provided to getElementSize does not exist in the requested ListFile_t.\n");
		exit(1);
	}
	struct Node_t* current = it->head;
	while(index)
	{
		current = getNext(current);
		index--;
	}
	return getNodeSize(current);
}

void* getElementData(struct ListFile_t* it, size_t index)
{
	if(index >= getSize(it))
    {
        fprintf(stderr, "The index provided to getElementData does not exist in the requested ListFile_t.\n");
        exit(1);
    }
    struct Node_t* current = it->head;
    while(index)
    {
        current = getNext(current);
        index--;
    }
    return getData(current);
}

const char* getElementName(struct ListFile_t* it, size_t index)
{
	if(index >= getSize(it))
    {
        fprintf(stderr, "The index provided to getElementData does not exist in the requested ListFile_t.\n");
        exit(1);
    }
    struct Node_t* current = it->head;
    while(index)
    {
        current = getNext(current);
        index--;
    }
    return getName(current);
}
