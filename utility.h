#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#ifdef WIN32
	#define CLEAR "cls"
#else
	#define CLEAR "clear"
#endif

//Clears the keyboard buffer, fflush
void clearKeyBuffer();

//Converts the integer number to a string stored in string, if failed returns NULL
void int_to_string(char *string, const int number);

//Clears the screen
void clearScreen();

//Counts the number of characters until the next char, \n, \0
int nextChar(const char *str, char ch);

//Checks each digit of the string to see if it represents a number
//Returns 0 if not and 1 if it is
int strIsDigit(char *str);

//Will make a string lowercase
//CAUTION: Changes are made directly to the string
void strToLower(char *string);

#endif

