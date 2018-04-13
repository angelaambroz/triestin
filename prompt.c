// cc -std=c99 -Wall prompt.c mpc.c -ledit -lm -o prompt
// Basic REPL 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

void add_history(char* unused) {}


// MAC, LINUX
#else
#include "editline/readline.h"
#endif

// Helper functions

long minimum(int a[] )

/* Use operator string to see which operation to perform */
// Compiling
long eval_op(long x, char* op, long y) {
  if (strcmp(op, "+") == 0) { return x + y; }
  if (strcmp(op, "-") == 0) { return x - y; }
  if (strcmp(op, "*") == 0) { return x * y; }
  if (strcmp(op, "/") == 0) { return x / y; }
  if (strcmp(op, "%") == 0) { return x % y; }
  if (strcmp(op, "^") == 0) { return pow(x, y); }
  if (strcmp(op, "min") == 0) { return ; }
  return 0;
}

long eval(mpc_ast_t* t) {
  
  /* If tagged as number return it directly. */ 
  if (strstr(t->tag, "numero")) {
    return atoi(t->contents);
  }
  
  /* The operator is always second child. */
  char* op = t->children[1]->contents;
  
  /* We store the third child in `x` */
  long x = eval(t->children[2]);
  
  /* Iterate the remaining children and combining. */
  int i = 3;
  while (strstr(t->children[i]->tag, "espr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }
  
  return x;  
}

long leaf_count(mpc_ast_t* t) {

}


// Polish notation, using mpc.h
int main(int argc, char** argv)
{

	/* Create Some Parsers */
	// Parsing
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

	  // REPL
	  while (1) {

	    /* Output our prompt */
	    char* input = readline("triestin> ");
	    add_history(input);

	    /* Attempt to Parse the user Input */
		mpc_result_t r;
		if (mpc_parse("<stdin>", input, Triestin, &r)) {
			/* On Success Print the AST */
			long result = eval(r.output);
			printf("%li\n", result);
			mpc_ast_delete(r.output);
		} else {
			/* Otherwise Print the Error */
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}

		free(input);

	  }

	 /* Undefine and Delete our Parsers */
	mpc_cleanup(4, Numero, Operatore, Espr, Triestin);

	return 0;

}





