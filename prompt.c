// Basic REPL 

#include <stdio.h>
#include <stdlib.h>

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

void add_history(char* unused) {}


// MAC, LINUX
#else
#include "editline/readline.h"
#endif


int main(int argc, char** argv)
{
	/* Print Version and Exit Information */
	puts("Triestin Version 0.0.0.0.1");
	puts("svilupatto con <3");
	puts("fracca Ctrl+c per fugire\n");

	  /* In a never ending loop */
	  while (1) {

	    /* Output our prompt */
	    char* input = readline("triestin> ");
	    add_history(input);

	    /* Echo input back to user */
	    printf("Ti me ga dito %s\n", input);

	    free(input);

	  }

	  return 0;

}





