#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZEOFTEXT	4098
#define SIZEOFCMD 	2048

typedef struct commandList
{
	int cmd;
	int param1;
	int param2;
	char *text;
	struct commandList *next;
} cmdList;

//Inserts a new element to the list
//takes the list head as an argument
//returns the new head of the list if the list was empty or the old head otherwise
cmdList *insert(cmdList *head, int cmd_in, int param1_in, int param2_in, char text_in[]);

//Deletes the last element of the list
//takes the list head as an argument
//returns the head if the list is not empty and NULL if it is
cmdList *delete(cmdList *head);

void dumpList(cmdList *head);

//This function will return a pointer to the specified by index list item
//The first item, pointed by head, is the item with index 0, the next one, head->next is 1 etc
//Takes the head of the list and the index number and returns a pointer if the item exists and NULL if not.
cmdList *lseek(cmdList *head, int index);

#endif

