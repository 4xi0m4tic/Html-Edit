#include "list.h"

cmdList *insert(cmdList *head, int cmd_in, int param1_in, int param2_in, char text_in[])
{
	cmdList *new = NULL, *tmp = head;

	new = (cmdList *)malloc(sizeof(cmdList));
	if(new == NULL)
	{
		puts("Couldn't allocate the memory for the list item.");
		return head;
	}

	new->cmd = cmd_in;
	new->param1 = param1_in;
	new->param2 = param2_in;
	new->text = (char *)malloc(SIZEOFTEXT * sizeof(char));
	if(new->text == NULL)
	{
		free(new);
		puts("Couldn't allocate the memory for the list text.");
		return head;
	}
	memset(new->text, '\0', SIZEOFTEXT);
	strncpy(new->text, text_in, SIZEOFTEXT - 1);
	new->next = NULL;

	if(head != NULL)
	{
		while(tmp->next != NULL)
			tmp = tmp->next;

		tmp->next = new;
		return head;
	}
	else
		return new;
}

cmdList *delete(cmdList *head)
{
	cmdList *tmp = head;

	if(head != NULL)
	{
		if(tmp->next != NULL)//If there are more than one objects in the list
		{
			while(tmp->next->next != NULL)
				tmp = tmp->next;

			free(tmp->next->text);
			free(tmp->next);
			tmp->next = NULL;
		}
		else//If the list has one object
		{
			free(tmp->text);
			free(tmp);
			return NULL;
		}
	}

	return head;
}

void dumpList(cmdList *head)
{
	cmdList *cursor = head;
	printf("\nhead = %x\n", head);
	while(cursor != NULL)
	{
		printf("\ncmd = %d\n", cursor->cmd);
		printf("param1 = %d\n", cursor->param1);
		printf("param2 = %d\n", cursor->param2);
		printf("text = %s\n", cursor->text);
		printf("next = %x\n", cursor->next);
		cursor = cursor->next;
	}
	putchar('\n');
}

cmdList *lseek(cmdList *head, int index)
{
	cmdList *cursor=head;
	int i;
	for(i=0;(cursor->next!=NULL)&&(i!=index);i++,cursor=cursor->next)
		;
	if(i<index)
		return NULL;
	else
		return cursor;
}
