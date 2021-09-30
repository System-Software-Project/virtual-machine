/* 
	This is the lex.c file for the UCF Fall 2021 Systems Software Project.
	For HW2, you must implement the function lexeme *lexanalyzer(char *input).
	You may add as many constants, global variables, and support functions
	as you desire.
	
	If you choose to alter the printing functions or delete list or lex_index, 
	you MUST make a note of that in you readme file, otherwise you will lose 
	5 points.
*/


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 500
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

// Global variables
lexeme *list;

// Used to find the current index and the index that should be right after
int lex_index = 0;
int look_ahead = 0;
int list_counter = 0;
int digit_counter = 1;
int temp_value = 0; 
char temp_value_array[5];
// Used for temporarily taking characters from the input string to compare to the if-else statments
char temp_string[12];
int temp_string_index = 0;

// Checks if we've currently hit a comment
int comment_flag = 0;

// Functional prototypes
void printlexerror(int type);
void printtokens();


lexeme *lexanalyzer(char *input)
{
	list = malloc(MAX_NUMBER_TOKENS * sizeof(lexeme));

	if (input == NULL)
	{
		return NULL;
	}

	while (input[lex_index] != '\0')
	{
		look_ahead = lex_index + 1;

		// Check if the input is on a comment
		if (input[lex_index] == '/' && input[look_ahead] == '/')
		{
			comment_flag = 1;
		}
		else if (iscntrl(input[look_ahead]))
		{
			comment_flag = 0;
		}
		if (comment_flag == 0)
		{
			// Grabbing characters to store into
			if (temp_string_index > 12)
			{
				printlexerror(4);
				return list;
			}
			if (isalpha(input[lex_index]) && isalpha(input[look_ahead]) != 0)
			{
				temp_string[temp_string_index] = input[lex_index];
				temp_string_index++;
			}
			else if (isalpha(input[lex_index]) && (isalpha(input[look_ahead] == 0 || isdigit(input[look_ahead] == 0))))
			{
				temp_string[temp_string_index] = input[lex_index];
				temp_string[temp_string_index + 1] = '\0';
				temp_string_index = 0;

				// Check to see if the temp string is equal to any reserved words
				if (strcmp(temp_string, "const") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 1;
					list_counter++;
				}
				else if (strcmp(temp_string, "var") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 2;
					list_counter++;
				}
				else if (strcmp(temp_string, "procedure") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 3;
					list_counter++;
				}
				else if (strcmp(temp_string, "call") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 11;
					list_counter++;
				}
				else if (strcmp(temp_string, "if") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 8;
					list_counter++;
				}
				else if (strcmp(temp_string, "then") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 9;
					list_counter++;
				}
				else if (strcmp(temp_string, "else") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 10;
					list_counter++;
				}
				else if (strcmp(temp_string, "while") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 6;
					list_counter++;
				}
				else if (strcmp(temp_string, "do") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 7;
					list_counter++;
				}
				else if (strcmp(temp_string, "begin") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 4;
					list_counter++;
				}
				else if (strcmp(temp_string, "end") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 5;
					list_counter++;
				}
				else if (strcmp(temp_string, "read") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 13;
					list_counter++;
				}
				else if (strcmp(temp_string, "write") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 12;
					list_counter++;
				}
				else if (strcmp(temp_string, "odd") == 0)
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 28;
					list_counter++;
				}
				// Chances are, it is an identifier if it is none of these
				else
				{
					strcpy(list[list_counter].name, temp_string);
					list[list_counter].type = 14;
					list_counter++;
				}
				temp_string[0] = '\0';
			}

			// Check for an assigner
			if (input[lex_index] == ':' && input[look_ahead] == '=')
			{
				strcpy(list[list_counter].name, ":=");	
				list[list_counter].type = 16;
				list_counter++;
			}
			else if (input[lex_index] == ':' && input[look_ahead] != '=')
			{
				printlexerror(1);
				return list;
			}

			// Check for an neq
			if (input[lex_index] == '!' && input[look_ahead] == '=')
			{
				strcpy(list[list_counter].name, "!=");	
				list[list_counter].type = 23;
				list_counter++;
			}
			else if (input[lex_index] == '!' && input[look_ahead] != '=')
			{
				printlexerror(1);
				return list;
			}

			// Check for an neq
			if (input[lex_index] == '=' && input[look_ahead] == '=')
			{
				strcpy(list[list_counter].name, "==");	
				list[list_counter].type = 22;
				list_counter++;
			}
			else if (input[lex_index] == '=' && input[look_ahead] != '=')
			{
				printlexerror(1);
				return list;
			}



			// Check for an geq or gtr
			if (input[lex_index] == '>' && input[look_ahead] == '=')
			{
				strcpy(list[list_counter].name, ">=");	
				list[list_counter].type = 27;
				list_counter++;
			}
			else if (input[lex_index] == '>' && isspace(input[look_ahead]))
			{
				strcpy(list[list_counter].name, ">");	
				list[list_counter].type = 26;
				list_counter++;
			}

			// Check for an leq or neq or less
			if (input[lex_index] == '<' && input[look_ahead] == '=')
			{
				strcpy(list[list_counter].name, "<=");	
				list[list_counter].type = 25;
				list_counter++;
			}
			else if (input[lex_index] == '<' && isspace(input[look_ahead]))
			{
				strcpy(list[list_counter].name, "<");	
				list[list_counter].type = 24;
				list_counter++;
			}

			// Check for a comma
			if (input[lex_index] == ',')
			{
				strcpy(list[list_counter].name, ",");	
				list[list_counter].type = 31;
				list_counter++;
			}

			// Check for a period
			if (input[lex_index] == '.')
			{
				strcpy(list[list_counter].name, ".");	
				list[list_counter].type = 32;
				list_counter++;
			}

			// Check for a semicolon
			if (input[lex_index] == ';')
			{
				strcpy(list[list_counter].name, ";");	
				list[list_counter].type = 33;
				list_counter++;
			}

			// Check for a semicolon
			if (input[lex_index] == ';')
			{
				strcpy(list[list_counter].name, ";");	
				list[list_counter].type = 33;
				list_counter++;
			}

			// Check for r parenthesis
			if (input[lex_index] == '(')
			{
				strcpy(list[list_counter].name, "(");	
				list[list_counter].type = 29;
				list_counter++;
			}

			// Check for l parenthesis
			if (input[lex_index] == ')')
			{
				strcpy(list[list_counter].name, ")");	
				list[list_counter].type = 30;
				list_counter++;
			}

			// Check for mod
			if (input[lex_index] == '%')
			{
				strcpy(list[list_counter].name, "%");	
				list[list_counter].type = 21;
				list_counter++;
			}

			// Check for mult
			if (input[lex_index] == '*')
			{
				strcpy(list[list_counter].name, "*");	
				list[list_counter].type = 19;
				list_counter++;
			}

			// Check for div
			if (input[lex_index] == '/' && isspace(input[look_ahead]))
			{
				strcpy(list[list_counter].name, "/");	
				list[list_counter].type = 20;
				list_counter++;
			}

			// Check for plus
			if (input[lex_index] == '+' && isspace(input[look_ahead]))
			{
				strcpy(list[list_counter].name, "+");	
				list[list_counter].type = 17;
				list_counter++;
			}

			// Check for minus
			if (input[lex_index] == '-' && isspace(input[look_ahead]))
			{
				strcpy(list[list_counter].name, "-");	
				list[list_counter].type = 18;
				list_counter++;
			}
			
			// Check for digit
			if (digit_counter > 5)
			{
				printlexerror(3);
				return list;
			}
			if (isdigit(input[lex_index]) && isdigit(look_ahead) && digit_counter <= 5)
			{
				temp_value_array[digit_counter - 1] = input[lex_index];
				digit_counter++;
			}
			else if (isdigit(input[lex_index]) && (isdigit(look_ahead) == 0 || isalpha(look_ahead) == 0) && digit_counter <= 5)
			{
				temp_value_array[digit_counter - 1] = input[lex_index];
				digit_counter = 0;
				temp_value = atoi(temp_value_array);
				strcpy(list[list_counter].name, temp_value_array);
				list[list_counter].value = temp_value;
				list[list_counter].type = 15;
			}
			else if (isdigit(input[lex_index - 1]) && (isdigit(look_ahead) || isalpha(look_ahead)) && digit_counter > 5)
			{
				printlexerror(3);
				return list;
			}

			

			
			lex_index++;
		}
		lex_index++;
	}
	if (list != NULL)
	{
		printtokens();
	}
	return list;
}


void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
			case oddsym:
				printf("%11s\t%d", "odd", oddsym);
				break;
			case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case neqsym:
				printf("%11s\t%d", "!=", neqsym);
				break;
			case lsssym:
				printf("%11s\t%d", "<", lsssym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case modsym:
				printf("%11s\t%d", "%", modsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case divsym:
				printf("%11s\t%d", "/", divsym);
				break;
			case addsym:
				printf("%11s\t%d", "+", addsym);
				break;
			case subsym:
				printf("%11s\t%d", "-", subsym);
				break;
			case lparensym:
				printf("%11s\t%d", "(", lparensym);
				break;
			case rparensym:
				printf("%11s\t%d", ")", rparensym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case assignsym:
				printf("%11s\t%d", ":=", assignsym);
				break;
			case beginsym:
				printf("%11s\t%d", "begin", beginsym);
				break;
			case endsym:
				printf("%11s\t%d", "end", endsym);
				break;
			case ifsym:
				printf("%11s\t%d", "if", ifsym);
				break;
			case thensym:
				printf("%11s\t%d", "then", thensym);
				break;
			case elsesym:
				printf("%11s\t%d", "else", elsesym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case constsym:
				printf("%11s\t%d", "const", constsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
		}
		printf("\n");
	}
	printf("\n");
	printf("Token List:\n");
	for (i = 0; i < lex_index; i++)
	{
		if (list[i].type == numbersym)
			printf("%d %d ", numbersym, list[i].value);
		else if (list[i].type == identsym)
			printf("%d %s ", identsym, list[i].name);
		else
			printf("%d ", list[i].type);
	}
	printf("\n");
	list[lex_index++].type = -1;
}

void printlexerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Excessive Number Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Excessive Identifier Length\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");
	
	free(list);
	return;
}