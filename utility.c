#include "utility.h"

void clearKeyBuffer()
{
	while(getchar() != '\n');
}

void int_to_string(char *string, const int number)
{
	int characters_written = 0;
	characters_written = sprintf(string, "%d", number);
	if(characters_written == 0)
		string = NULL;
}

void clearScreen()
{
	system(CLEAR);
}

int nextChar(const char *str, char ch)
{
	int i;
	for(i = 0; (str[i] != ch) && (str[i] != '\n') && (str[i] != '\0'); i++);

	return i;
}

int strIsDigit(char *str)
{
	int i;
	for(i = 0; (!iscntrl(str[i])) && (i <= strlen(str)); i++)
		if(!isdigit(str[i]))
			return 0;

	return 1;
}

void strToLower(char *string)
{
	int i;
	for(i = 0; string[i] != '\0'; i++)
		string[i] = tolower(string[i]);
}


