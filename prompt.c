// cc -std=c99 -Wall prompt.c mpc.c -ledit -lm -o prompt
// Basic REPL 

#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

// WINDOWS

#ifdef _WIN32
#include <string.h>

static char buffer[2048];

char* readline(char* prompt) 
{
	fputs(prompt, stdout);
	fgets(buffer, 2048, stdin);
	char* cpy = malloc(strlen(buffer)+1);
	strcpy(cpy, buffer);
	cpy[strlen(cpy)-1] = '\0';
	return cpy;
}

long peak(char* numbers, bool min)
{
	// iterature through the ints in the array, 4 bytes at a time
	// how do i know the length of the array? oof
	// TODO: FIX THIS
	int size_of_numbers = 10;

	// start somewhere: the beginning
	long peak = numbers[0];

	for (c = 0; c < size_of_numbers; c++)
	{
		if (min)
		{
			if (numbers[c] < peak)
			{
				peak = numbers[c];
			}
		}
		else
		{
			if (numbers[c] > peak)
			{
				peak = numbers[c];
			}
		}
	}

	return peak;

}

void add_history(char* unused) {}


// MAC, LINUX
#else
#include "editline/readline.h"
#endif


// Polish notation, using mpc.h
int main(int argc, char** argv)
{

	/* Create Some Parsers */
	mpc_parser_t* Numero   = mpc_new("numero");
	mpc_parser_t* Operatore = mpc_new("operatore");
	mpc_parser_t* Espr     = mpc_new("espr");
	mpc_parser_t* Triestin    = mpc_new("triestin");

	/* Define them with the following Language */
	mpca_lang(MPCA_LANG_DEFAULT,
	  "                                                     \
	    numero   : /-?[0-9]+/ ;                             \
	    operatore : '+' | '-' | '*' | '/' ;                  \
	    espr     : <numero> | '(' <operatore> <espr>+ ')' ;  \
	    triestin    : /^/ <operatore> <espr>+ /$/ ;             \
	  ",
	  Numero, Operatore, Espr, Triestin);

	/* Print Version and Exit Information */
	puts("Triestin Version 0.0.0.0.1");
	puts("svilupatto con <3");
	puts("fracca Ctrl+c per fugire\n");

	  /* In a never ending loop */
	  while (1) {

	    /* Output our prompt */
	    char* input = readline("triestin> ");
	    add_history(input);

	    // /* Echo input back to user */
	    // printf("Ti me ga dito %s\n", input);

	    // free(input);

	    /* Attempt to Parse the user Input */
		mpc_result_t r;
		if (mpc_parse("<stdin>", input, Triestin, &r)) {
		  /* On Success Print the AST */
		  mpc_ast_print(r.output);
		  mpc_ast_delete(r.output);
		} else {
		  /* Otherwise Print the Error */
		  mpc_err_print(r.error);
		  mpc_err_delete(r.error);
		}

	  }

	 /* Undefine and Delete our Parsers */
	mpc_cleanup(4, Numero, Operatore, Espr, Triestin);

	  return 0;

}





