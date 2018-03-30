#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"
#include "utility.h"

#define TITLE		0 //newTitle [text]
#define HEADING		1 //newHeading X [text]
#define LINE		2 //newLine
#define PAR		3 //newPar [text]
#define TABLE		4 //newTable X Y [text]
#define BLIST		5 //newBlist X [text]
#define NLIST		6 //newNlist X [text]

cmdList *getcommand(cmdList *head, char cmd[], int *status);
char *isEmail(char *str);
char *isHour(char *str);
char *isDate(char *str);
char *isTelephone(char *str);
int countCommas(char *str);
void printListItem(cmdList *item);
int applyStyles(char *string);
int extractCommand(cmdList *cmd, FILE *out);
void logo();

int main(int argc, char **argv)
{
	int status = 0;
	cmdList *head = NULL;
	char cmd[SIZEOFCMD] = {0};

	if((argc == 5) && (strcmp(argv[1], "-i") == 0) && (strstr(argv[2],".txt") == NULL) && (strcmp(argv[3], "-n") == 0)) //name -i path_for_input_without_txt -n number_of_commands
	{
		strcpy(cmd, "load ");
		strcat(cmd, argv[4]);
		strcat(cmd, " ");
		strcat(cmd, argv[2]);
		strcat(cmd, ".txt");

		head = getcommand(head, cmd, &status);
		head = getcommand(head, "save a", &status);
	}
	else if((argc == 7) && (strcmp(argv[1], "-i") == 0) && (strstr(argv[2],".txt") == NULL) && (strcmp(argv[3], "-n") == 0) && (strcmp(argv[5], "-o") == 0) && (strstr(argv[6],".html") == NULL)) //name -i path_for_input -n number_of_commands -o path_for_output_without_html
	{
		strcpy(cmd, "load ");
		strcat(cmd, argv[4]);
		strcat(cmd, " ");
		strcat(cmd, argv[2]);
		strcat(cmd, ".txt");

		head = getcommand(head, cmd, &status);
		
		strcpy(cmd, "save ");
		strcat(cmd, argv[6]);

		head = getcommand(head, cmd, &status);
	}
	else if((argc == 2) && (strcmp(argv[1], "-console") == 0)) //name -console
	{
		clearScreen();
		logo();

		while(1)
		{
			printf("$> ");
			scanf("%[^\n]", &cmd);

			head = getcommand(head, cmd, &status);

			if(strstr(cmd, "exit"))
				break;

			clearKeyBuffer();
			memset(cmd, '\0', sizeof(cmd));
			status = 0;
		}
	}
	else
	{
		printf("Syntax:\n");
		printf("  %s -i [path to a txt file (without the .txt)] -n [number of commands to load]\n", argv[0]);
		printf("  %s -i [path to a txt file (without the .txt)] -n [number of commands to load]\n", argv[0]);
		printf("                   -o [path to where the file will be saved(without the .html)]\n");
		printf("  %s -console", argv[0]);
	}

	return 0;
}

//Translates the inputed string to a new cmdList struct and adds it to the list
//Returns the new head if succeeded and the old one if failed.
//The status integer will be set to 1 if the operation suceeded and 0 if not
cmdList *getcommand(cmdList *head, char cmd[], int *status)
{
	cmdList *new_head;
	char tmp_text[SIZEOFTEXT] = {0};
	char tmp_param1[SIZEOFTEXT] = {0};
	char tmp_param2[SIZEOFTEXT] = {0};

	if(strstr(cmd,"newTitle ") != NULL)
	{
		sscanf(cmd, "newTitle %[^\n]", tmp_text);

		if(strlen(tmp_text) != 0)
		{
			new_head = insert(head,TITLE, 0, 0, tmp_text);
			*status = 1;
		}
		else
		{
			puts("$> The text must not be empty.");

			*status = 0;
			return head;
		}
	}
	else if(strstr(cmd,"newHeading ") != NULL)
	{
		sscanf(cmd,"newHeading %s %[^\n]", tmp_param1, tmp_text);

		if(atoi(tmp_param1) >= 1 && atoi(tmp_param1) <= 6)
		{
			if(strlen(tmp_text) != 0)
			{
				new_head = insert(head, HEADING, atoi(tmp_param1), 0, tmp_text);
				*status = 1;
			}
			else
			{
				puts("$> The text must not be empty.");
				*status = 0;
				return head;
			}
		}
		else
		{
			puts("$> The first parameter must be between 1 and 6.");
			*status = 0;
			return head;
		}
	}
	else if(strstr(cmd,"newLine") != NULL)
	{
		new_head = insert(head, LINE, 0, 0, "");
		*status = 1;
	}
	else if(strstr(cmd,"newPar ") != NULL)
	{
		sscanf(cmd ,"newPar %[^\n]", tmp_text);

		if(strlen(tmp_text) != 0)
		{
			new_head = insert(head, PAR, 0, 0, tmp_text);
			*status = 1;
		}
		else
		{
			puts("$> The text must not be empty.");
			*status = 0;
			return head;
		}
	}
	else if(strstr(cmd,"newTable ") != NULL)
	{
		sscanf(cmd ,"newTable %s %s %[^\n]", tmp_param1, tmp_param2, tmp_text);

		if(atoi(tmp_param1) >= 1)
		{
			if(atoi(tmp_param2) >= 1)
			{
				if((strlen(tmp_text) != 0))
				{
					if((atoi(tmp_param1) * atoi(tmp_param2)) - 1 == countCommas(tmp_text))
					{
						new_head = insert(head,TABLE, atoi(tmp_param1), atoi(tmp_param2), tmp_text);
						*status = 1;
					}
					else
					{
						puts("$> There aren't enough values to fill the table.");
						*status = 0;
						return head;
					}
				}
				else
				{
					puts("$> The text must not be empty.");
					*status = 0;
					return head;
				}
			}
			else
			{
				puts("$> The second parameter must be greater or equal to 1.");
				*status = 0;
				return head;
			}
		}
		else
		{
			puts("$> The first parameter must be greater or equal to 1.");
			*status = 0;
			return head;
		}
	}
	else if(strstr(cmd,"newBlist ") != NULL)
	{
		sscanf(cmd ,"newBlist %s %[^\n]", tmp_param1, tmp_text);

		if(atoi(tmp_param1) >= 1)
		{
			if(strlen(tmp_text) != 0)
			{
				if(atoi(tmp_param1) - 1 == countCommas(tmp_text))
				{
					new_head = insert(head, BLIST, atoi(tmp_param1), 0, tmp_text);
					*status = 1;
				}
				else
				{
					puts("$> There aren't enough values to fill the list.");
					*status = 0;
					return head;
				}
			}
			else
			{
				puts("$> The text must not be empty.");
				*status = 0;
				return head;
			}
		}
		else
		{
			puts("$> The first parameter must be greater or equal to 1.");
			*status = 0;
			return head;
		}
	}
	else if(strstr(cmd,"newNlist ") != NULL)
	{
		sscanf(cmd ,"newNlist %s %[^\n]", tmp_param1, tmp_text);

		if(atoi(tmp_param1) >= 1)
		{
			if(strlen(tmp_text) != 0)
			{
				if(atoi(tmp_param1) - 1 == countCommas(tmp_text))
				{
					new_head = insert(head, NLIST, atoi(tmp_param1), 0, tmp_text);
					*status = 1;
				}
				else
				{
					puts("$> There aren't enough values to fill the list.");
					*status = 0;
					return head;
				}
			}
			else
			{
				puts("$> The text must not be empty.");
				*status = 0;
				return head;
			}
		}
		else
		{
			puts("$> The first parameter must be greater or equal to 1.");
			*status = 0;
			return head;
		}
	}
	else if(strstr(cmd, "clearScreen"))
	{
		clearScreen();
		*status = 1;
		return head;
	}
	else if(strstr(cmd, "undo"))
	{
		new_head = delete(head);
		*status = 1;
	}
	else if(strstr(cmd, "clear"))
	{
		new_head = head;
		while(new_head != NULL)
			new_head = delete(new_head);

		*status = 1;
	}
	else if(strstr(cmd, "print-l2f "))
	{
		int i;
		cmdList *cursor = NULL;
		sscanf(cmd ,"print-l2f %s", tmp_param1);

		if(atoi(tmp_param1) < 0)
		{
			puts("$> The first parameter must be positive.");
			*status = 0;
		}
		else
		{
			for(i = atoi(tmp_param1) - 1; i >= 0; i--)
			{
				cursor = lseek(head, i);
				if(cursor != NULL)
					printListItem(cursor);
			}
			*status = 1;
		}

		return head;
	}
	else if(strstr(cmd, "print-f2l "))
	{
		int i;
		cmdList *cursor = NULL;
		sscanf(cmd ,"print-f2l %s", tmp_param1);

		if(atoi(tmp_param1) < 0)
		{
			puts("$> The first parameter must be positive.");
			*status = 0;
		}
		else
		{
			for(i = 0; i < atoi(tmp_param1); i++)
			{
				cursor = lseek(head, i);
				if(cursor != NULL)
					printListItem(cursor);
			}

			*status = 1;
		}

		return head;
	}
	else if(strstr(cmd, "show "))
	{
		cmdList *cursor = head;
		sscanf(cmd ,"show %s", tmp_text);
		strToLower(tmp_text);
		while(cursor != NULL)
		{
			if(cursor->cmd == TITLE)
			{
				if(strstr("newtitle", tmp_text) != NULL)
					printListItem(cursor);
			}
			else if(cursor->cmd == HEADING)
			{
				if(strstr("newhead", tmp_text) != NULL)
					printListItem(cursor);
			}
			else if(cursor->cmd == LINE)
			{
				if(strstr("newline", tmp_text) != NULL)
					printListItem(cursor);
			}
			else if(cursor->cmd == PAR)
			{
				if(strstr("newpar", tmp_text) != NULL)
					printListItem(cursor);
			}
			else if(cursor->cmd == TABLE)
			{
				if(strstr("newtable", tmp_text) != NULL)
					printListItem(cursor);
			}
			else if(cursor->cmd == BLIST)
			{
				if(strstr("newblist", tmp_text) != NULL)
					printListItem(cursor);
			}
			else if(cursor->cmd == NLIST)
			{
				if(strstr("newnlist", tmp_text) != NULL)
					printListItem(cursor);
			}

			cursor = cursor->next;
		}
		*status = 1;
		return head;
	}
	else if(strstr(cmd, "exit"))
	{
		new_head = head;
		while(new_head != NULL)
			new_head = delete(new_head);

		*status = 1;
		return head;
	}
	else if(strstr(cmd, "help"))
	{
		puts("$>  Supported commands:");
		puts("$>    newTitle [text] : Adds a new <TITLE>.");
		puts("$>    newHeading x [text] : Adds a new <Hx> tag containing the text. The x must be between 1 and 6.");
		puts("$>    newLine : Adds a new <HR> line.");
		puts("$>    newPar [text] : Adds a new <P> tag containing the text.");
		puts("$>    newTable x y [text] : Adds a new <TABLE> with x rows and y columns. The text elements must be seperated with ',' .");
		puts("$>    newBlist x [text] : Adds a new <UL> list with x elements. The text elements must be seperated with ',' .");
		puts("$>    newNlist x [text] : Adds a new <OL> list with x elements. The text elements must be seperated with ',' .");
		puts("$>    clearScreen : Clears the screen.");
		puts("$>    undo : Undo the last command.");
		puts("$>    clear : Clears the whole command history.");
		puts("$>    print-f2l x : Prints the first x commands.");
		puts("$>    print-l2f x : Prints the last x commands.");
		puts("$>    show [text] : Prints all the commands that contain the text.");
		puts("$>    save [filename] : Saves all the commands to an html file. The filename must not contain the .html .");
		puts("$>    exit : Terminates the program.");
		puts("$>    load x [path of a txt file] : Loads the first x commands from the txt file.");
		*status = 1;
		return head;
	}
	else if(strstr(cmd, "load "))
	{
		int i, statusInternal = 0;
		char buffer[SIZEOFCMD]={0};
		sscanf(cmd ,"load %s %s", &tmp_param1,tmp_text);
		if(atoi(tmp_param1)<=0)
		{
			puts("$> The first parameter must be positive.");
			*status = 0;
			return head;
		}
		else
		{
			new_head = head;
			FILE *input;
			input = fopen(tmp_text,"r");

			if(input == NULL)
			{
				puts("$> Cannot open the file.");
				*status = 0;
				fclose(input);
				return head;
			}
			else
			{
				for(i = 0; (!feof(input)) && (i < atoi(tmp_param1)); i++)
				{
					if(!fgets(buffer, SIZEOFCMD, input))
					{
						puts("$> Couldn't get the next line.");
						*status = 0;
						fclose(input);
						return head;
					}
					else
					{
						//cmdList *getcommand(cmdList *head, char cmd[])
						new_head = getcommand(new_head, buffer, &statusInternal);

						if(statusInternal == 0)
						{
							printf("$> There may be an error in command %d.\n", i + 1);
							*status = 0;
							fclose(input);
							return new_head;
						}

						//clearKeyBuffer();
						memset(buffer, '\0', sizeof(buffer));
					}
				}
				*status = 1;
				fclose(input);
			}
		}
	}
	else if(strstr(cmd, "save "))
	{
		sscanf(cmd ,"save %s", tmp_text);
		if(strstr(tmp_text, ".html"))
		{
			puts("$> The filename must not contain the .html .");
			*status = 0;
			return head;
		}
		else
		{
			FILE *fp;
			strcat(tmp_text, ".html");
			fp = fopen(tmp_text, "w");
			
			fprintf(fp, "<!-- This document was generated with htmledit -->\n");
			fprintf(fp, "<html>\n<head>\n");

			cmdList *cursor = head, *title = NULL;
			if(cursor != NULL)
			{
				while(cursor != NULL)
				{
					if(cursor->cmd == TITLE)
						title = cursor;
					
					cursor = cursor->next;
				}
			}
			else
			{
				puts("$> There weren't any commands to save.");
				*status = 0;
				fclose(fp);
				return head;
			}
			
			if(title != NULL)
				extractCommand(title, fp);
			fprintf(fp, "</head>\n<body>\n");
			
			cursor = head;
			while(cursor != NULL)
			{
				if(cursor->cmd != TITLE)
					extractCommand(cursor, fp);
				
				cursor = cursor->next;
			}
			
			fprintf(fp, "<body>\n</html>\n");
			fclose(fp);
		}
	}
	else
	{
		puts("$> There was an invalid command.");
		*status = 0;
		return head;
	}

	return new_head;
}

char *isDate(char *str)
{
	char day[3] = {0}, month[3] = {0}, year[5] = {0};
	char *start = str;
	while(start <= str + strlen(str))
	{
		sscanf(start, "%2[^ /]/%2[^ /]/%4[^ ]", &day, &month, &year);
		if((strcmp(day, "") == NULL) || (strcmp(month, "") == NULL) || (strcmp(year, "") == NULL))
			start += nextChar(start, ' ') + 1;
		else
		{
			if((atoi(day) >= 1) && (atoi(day) <= 31) && (atoi(month) >= 1) && (atoi(month) <= 12) && (atoi(year) >= 0) && (atoi(year) <= 9999))
				return start;
			else
				start += nextChar(start, ' ') + 1;
		}
		memset(day, '\0', sizeof(day));
		memset(month, '\0', sizeof(month));
		memset(year, '\0', sizeof(year));
	}

	return NULL;
}

char *isTelephone(char *str)
{
	char code[5] = {0}, number[8] = {0};
	char *start = str;
	while(start <= str + strlen(str))
	{
		sscanf(start, "%4[^ -]-%7[^ ]", &code, &number);
		if((strcmp(code, "") == NULL) || (strcmp(number, "") == NULL))
			start += nextChar(start, ' ') + 1;
		else
		{
			if(strIsDigit(code) && strIsDigit(number))
			{
				if(((strlen(code) == 4) && (strlen(number) == 6)) || ((strlen(code) == 3) && (strlen(number) == 7)))
					return start;
				else
					start += nextChar(start, ' ') + 1;
			}
			else
				start += nextChar(start, ' ') + 1;
		}
		memset(code, '\0', sizeof(code));
		memset(number, '\0', sizeof(number));
	}

	return NULL;
}

char *isEmail(char *str)
{
	char name[1024] = {0},service[1024] = {0},domain[1024] = {0};
	char *start = str;
	while(start <= str + strlen(str))
	{
		sscanf(start, "%[^ @]@%[^ .].%[^ ]", &name, &service, &domain);
		if((strcmp(name, "") == NULL) || (strcmp(service, "") == NULL) || (strcmp(domain, "") == NULL))
			start += nextChar(start, ' ') + 1;
		else
		{
			if((strlen(domain) == 2) && (strlen(name) >= 3) && (isalpha(name[0])) && (isalpha(name[1])))
				return start;
			else
				start += nextChar(start, ' ') + 1;
		}
		memset(name, '\0', sizeof(name));
		memset(service, '\0', sizeof(service));
		memset(domain, '\0', sizeof(domain));
	}

	return NULL;
}

char *isHour(char *str)
{
	char hour[3] = {0}, min[3] = {0};
	char *start = str;
	while(start <= str + strlen(str))
	{
		sscanf(start, "%2[^ :]:%2[^ ]", &hour, &min);
		if((strcmp(hour, "") == NULL) || (strcmp(min, "") == NULL))
			start += nextChar(start, ' ') + 1;
		else
		{
			if((atoi(hour) >= 0) && (atoi(hour) <= 23) && (atoi(min) >= 0) && (atoi(min) <= 59))
				return start;
			else
				start += nextChar(start, ' ') + 1;
		}

		memset(hour, '\0', sizeof(hour));
		memset(min, '\0', sizeof(min));
	}

	return NULL;
}

int countCommas(char *str)
{
	int commas = 0, i;
	for(i = 0; i <= strlen(str); i++)
		if(str[i] == ',')
			commas++;

	return commas;
}

void printListItem(cmdList *item)
{
	if(item->cmd == TITLE)
		printf("$> newTitle %s\n", item->text);
	else if(item->cmd == HEADING)
		printf("$> newHeading %d %s\n", item->param1, item->text);
	else if(item->cmd == LINE)
		printf("$> newLine\n");
	else if(item->cmd == PAR)
		printf("$> newPar %s\n", item->text);
	else if(item->cmd == TABLE)
		printf("$> newTable %d %d %s\n", item->param1, item->param2, item->text);
	else if(item->cmd == BLIST)
		printf("$> newBlist %d %s\n", item->param1, item->text);
	else if(item->cmd == NLIST)
		printf("$> newNlist %d %s\n", item->param1, item->text);
}

//Takes a string and applies the required html tags to style the text
//CAUTION : Changes the string directly
//Returns 1 if there was a change and 0 if there wasn't
int applyStyles(char *string)
{
	char *style = NULL, tmp[SIZEOFTEXT] = {0};

	if((style = isEmail(string)) && (style != NULL))
	{
		strncpy(tmp, string, (style - string));
		strcat(tmp, "<font color=\"red\">");
		strncat(tmp, style, nextChar(style, ' '));
		strcat(tmp, "</font>");
		strncat(tmp, style + nextChar(style, ' '), SIZEOFTEXT - strlen(tmp));
		strcpy(string, tmp);
		return 1;
	}
	else if((style = isDate(string)) && (style != NULL))
	{
		strncpy(tmp, string, (style - string));
		strcat(tmp, "<font color=\"blue\">");
		strncat(tmp, style, nextChar(style, ' '));
		strcat(tmp, "</font>");
		strncat(tmp, style + nextChar(style, ' '), SIZEOFTEXT - strlen(tmp));
		strcpy(string, tmp);
		return 1;
	}
	else if((style = isHour(string)) && (style != NULL))
	{
		strncpy(tmp, string, (style - string));
		strcat(tmp, "<font color=\"green\">");
		strncat(tmp, style, nextChar(style, ' '));
		strcat(tmp, "</font>");
		strncat(tmp, style + nextChar(style, ' '), SIZEOFTEXT - strlen(tmp));
		strcpy(string, tmp);
		return 1;
	}
	else if((style = isTelephone(string)) && (style != NULL))
	{
		strncpy(tmp, string, (style - string));
		strcat(tmp, "<font color=\"orange\">");
		strncat(tmp, style, nextChar(style, ' '));
		strcat(tmp, "</font>");
		strncat(tmp, style + nextChar(style, ' '), SIZEOFTEXT - strlen(tmp));
		strcpy(string, tmp);
		return 1;
	}

	return 0;
}

//Writes the command described in the cmdList in the html file
//Returns 1 if it succeeded and 0 if not
int extractCommand(cmdList *cmd, FILE *out)
{
	int i, j;
	char tmp[SIZEOFTEXT] = {0};
	char *cursor = NULL;
	if(cmd->cmd == TITLE)
	{
		while(applyStyles(cmd->text) != 0);
		if(fprintf(out, "\t\t<title>%s</title>\n", cmd->text) < 0)
			return 0;
		else
			return 1;
	}
	else if(cmd->cmd == HEADING)
	{
		while(applyStyles(cmd->text) != 0);
		if(fprintf(out, "\t<h%d>%s</h%d>\n", cmd->param1, cmd->text, cmd->param1) < 0)
			return 0;
		else
			return 1;
	}
	else if(cmd->cmd == LINE)
	{
		if(fprintf(out, "\t<hr>\n") < 0)
			return 0;
		else
			return 1;
	}
	else if(cmd->cmd == PAR)
	{
		while(applyStyles(cmd->text) != 0);
		if(fprintf(out, "\t<p>%s</p>\n", cmd->text) < 0)
			return 0;
		else
			return 1;
	}
	else if(cmd->cmd == TABLE)
	{
		cursor = cmd->text;
		fprintf(out, "\t<table>\n");
		for(i = 0; i < cmd->param1; i++)
		{
			fprintf(out, "\t\t<tr>\n");
			for(j = 0; j < cmd->param2; j++)
			{
				strncpy(tmp, cursor, nextChar(cursor, ','));
				cursor += nextChar(cursor, ',') + 1;
				while(applyStyles(tmp) != 0);
				fprintf(out, "\t\t\t<td>%s</td>\n", tmp);
				memset(tmp, '\0', SIZEOFTEXT);
			}
			fprintf(out, "\t\t</tr>\n");
		}
		fprintf(out, "\t</table>\n");
	}
	else if(cmd->cmd == BLIST)
	{
		cursor = cmd->text;
		fprintf(out, "\t<ul>\n");
		for(i = 0; i < cmd->param1; i++)
		{	
			strncpy(tmp, cursor, nextChar(cursor, ','));
			cursor += nextChar(cursor, ',') + 1;
			while(applyStyles(tmp) != 0);
			fprintf(out, "\t\t<li>%s</li>\n", tmp);
			memset(tmp, '\0', SIZEOFTEXT);
		}
		fprintf(out, "\t</ul>\n");
	}
	else if(cmd->cmd == NLIST)
	{
		cursor = cmd->text;
		fprintf(out, "\t<ol>\n");
		for(i = 0; i < cmd->param1; i++)
		{	
			strncpy(tmp, cursor, nextChar(cursor, ','));
			cursor += nextChar(cursor, ',') + 1;
			while(applyStyles(tmp) != 0);
			fprintf(out, "\t\t<li>%s</li>\n", tmp);
			memset(tmp, '\0', SIZEOFTEXT);
		}
		fprintf(out, "\t</ol>\n");
	}
}

void logo()
{
	puts("$> __/\\\\\\_______________________________________________/\\\\\\\\\\\\_____________________________/\\\\\\________________________");
	puts("$>  _\\/\\\\\\______________________________________________\\////\\\\\\____________________________\\/\\\\\\________________________");
	puts("$>   _\\/\\\\\\______________/\\\\\\_______________________________\\/\\\\\\____________________________\\/\\\\\\____/\\\\\\______/\\\\\\______");
	puts("$>    _\\/\\\\\\___________/\\\\\\\\\\\\\\\\\\\\\\_____/\\\\\\\\\\__/\\\\\\\\\\_______\\/\\\\\\_________/\\\\\\\\\\\\\\\\__________\\/\\\\\\___\\///____/\\\\\\\\\\\\\\\\\\\\\\_");
	puts("$>     _\\/\\\\\\\\\\\\\\\\\\\\___\\////\\\\\\////____/\\\\\\///\\\\\\\\\\///\\\\\\_____\\/\\\\\\_______/\\\\\\/////\\\\\\____/\\\\\\\\\\\\\\\\\\____/\\\\\\__\\////\\\\\\////__");
	puts("$>      _\\/\\\\\\/////\\\\\\_____\\/\\\\\\_______\\/\\\\\\_\\//\\\\\\__\\/\\\\\\_____\\/\\\\\\______/\\\\\\\\\\\\\\\\\\\\\\____/\\\\\\////\\\\\\___\\/\\\\\\_____\\/\\\\\\______");
	puts("$>       _\\/\\\\\\___\\/\\\\\\_____\\/\\\\\\_/\\\\___\\/\\\\\\__\\/\\\\\\__\\/\\\\\\_____\\/\\\\\\_____\\//\\\\///////____\\/\\\\\\__\\/\\\\\\___\\/\\\\\\_____\\/\\\\\\_/\\\\__");
	puts("$>        _\\/\\\\\\___\\/\\\\\\_____\\//\\\\\\\\\\____\\/\\\\\\__\\/\\\\\\__\\/\\\\\\___/\\\\\\\\\\\\\\\\\\___\\//\\\\\\\\\\\\\\\\\\\\__\\//\\\\\\\\\\\\\\/\\\\__\\/\\\\\\_____\\//\\\\\\\\\\___");
	puts("$>         _\\///____\\///_______\\/////_____\\///___\\///___\\///___\\/////////_____\\//////////____\\///////\\//___\\///_______\\/////____");
}


