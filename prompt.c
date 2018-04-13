// cc -std=c99 -Wall prompt.c mpc.c -ledit -lm -o prompt
// Basic REPL 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpc.h"

// WINDOWS

// #ifdef _WIN32

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

/* Create Enumeration of Possible Error Types */
enum { TERR_DIV_ZERO, TERR_BAD_OP, TERR_BAD_NUM };

/* Create Enumeration of Possible tval Types */
enum { TVAL_NUM, TVAL_ERR, TVAL_SIM, TVAL_SESPR };

/* Declare New tval Struct */
typedef struct {
  int type;
  long num;
  char* err;
  char* sim;
  int count;
  struct tval** cell;
} tval;

/* Create a new number type tval */
tval* tval_num(long x) {
  tval* v = malloc(sizeof(tval));
  v->type = TVAL_NUM;
  v->num = x;
  return v;
}

/* Create a new error type tval */
tval* tval_err(char* m) {
  tval* v = malloc(sizeof(tval));
  v->type = TVAL_ERR;
  v->err = malloc(strlen(m) + 1);
  strcpy(v->err, m);
  return v;
}

tval* tval_sim(char* s) {
	tval* v = malloc(sizeof(tval));
	v->type = TVAL_SIM;
	v->sim = malloc(strlen(s) + 1);
	strcpy(v->sim, s);
	return v;
}

tval* tval_sespr(void) {
	tval* v = malloc(sizeof(tval));
	v->type = TVAL_SESPR;
	v->count = 0;
	v->cell = NULL;
	return v;
}

void tval_del(tval* v) {

	switch (v->type) {
		case TVAL_NUM: break;
		case TVAL_ERR: free(v->err); break;
		case TVAL_SIM: free(v->sim); break;
		case TVAL_SESPR:
			for (int i = 0; i < v->count; i++) {
				lval_del(v->cell[i])
			}
		free(v->cell);
		break;
	}

	free(v);
}

/* Print a "tval" */
void tval_print(tval v) {
  switch (v.type) {
    /* In the case the type is a number print it */
    /* Then 'break' out of the switch. */
    case TVAL_NUM: printf("%li", v.num); break;

    /* In the case the type is an error */
    case TVAL_ERR:
      /* Check what type of error it is and print it */
      if (v.err == TERR_DIV_ZERO) {
        printf("Errore: Ma chi ti ga dito che si puo dividere con zero?");
      }
      if (v.err == TERR_BAD_OP)   {
        printf("Errore: Operatore invalido");
      }
      if (v.err == TERR_BAD_NUM)  {
        printf("Errore: Non e' un vero numero");
      }
    break;
  }
}

/* Print a "tval" followed by a newline */
void tval_println(tval v) { tval_print(v); putchar('\n'); }

// Helper functions

/* Use operator string to see which operation to perform */
// Compiling
tval eval_op(tval x, char* op, tval y) {

	/* If either value is an error return it */
	if (x.type == TVAL_ERR) { return x; }
	if (y.type == TVAL_ERR) { return y; }

	/* Otherwise do maths on the number values */
	if (strcmp(op, "+") == 0) { return tval_num(x.num + y.num); }
	if (strcmp(op, "-") == 0) { return tval_num(x.num - y.num); }
	if (strcmp(op, "*") == 0) { return tval_num(x.num * y.num); }
	if (strcmp(op, "/") == 0) {
	/* If second operand is zero return error */
	return y.num == 0
	  ? tval_err(TERR_DIV_ZERO)
	  : tval_num(x.num / y.num);
	}
	if (strcmp(op, "%") == 0) { return tval_num(x.num % y.num); }
	if (strcmp(op, "^") == 0) { return tval_num(pow(x.num, y.num)); }

	return tval_err(TERR_BAD_OP);
}

tval eval(mpc_ast_t* t) {
  
  /* If tagged as number return it directly. */ 
  if (strstr(t->tag, "numero")) {
  	errno = 0;
  	long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? tval_num(x) : tval_err(TERR_BAD_NUM);
  }
  
  /* The operator is always second child. */
  char* op = t->children[1]->contents;
  
  /* We store the third child in `x` */
  tval x = eval(t->children[2]);
  
  /* Iterate the remaining children and combining. */
  int i = 3;
  while (strstr(t->children[i]->tag, "espr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }
  
  return x;  
}

// Polish notation, using mpc.h
int main(int argc, char** argv)
{

	/* Create Some Parsers */
	// Parsing
	mpc_parser_t* Numero   = mpc_new("numero");
	mpc_parser_t* Simbolo = mpc_new("simbolo");
	mpc_parser_t* Espr     = mpc_new("espr");
	mpc_parser_t* Sespr     = mpc_new("sespr");
	mpc_parser_t* Triestin    = mpc_new("triestin");

	/* Define them with the following Language */
	mpca_lang(MPCA_LANG_DEFAULT,
	  "                                                     \
	    numero   : /-?[0-9]+/ ;                             \
	    symbol : '+' | '-' | '*' | '/' | '%' ; \
	    sespr : '(' <espr>* ')' ; \
	    espr     : <numero> | <simbolo> | <sespr> ;  \
	    triestin    : /^/ <espr>* /$/ ;             \
	  ",
	  Numero, Simbolo, Sespr, Espr, Triestin);

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
			tval result = eval(r.output);
			tval_println(result);
			mpc_ast_delete(r.output);
		} else {
			/* Otherwise Print the Error */
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}

		free(input);

	  }

	 /* Undefine and Delete our Parsers */
	mpc_cleanup(5, Numero, Simbolo, Sespr, Espr, Triestin);

	return 0;

}





