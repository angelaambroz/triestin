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
// Linked list then?
typedef struct tval {
  int type;
  long num;
  char* err;
  char* sim;
  int count;
  struct tval** cell;
} tval;

void tval_print(tval* v);
void tval_println(tval* v);

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
				tval_del(v->cell[i]);
			}
		free(v->cell);
		break;
	}

	free(v);
}

/* Print a "tval" */
void tval_espr_print(tval* v, char open, char close) {
	putchar(open);
	for (int i = 0; i < v->count; i++) {
		tval_print(v->cell[i]);

		if (i != (v->count-1)) {
			putchar(' ');
		}
	}
	putchar(close);
}

void tval_print(tval* v) {
	switch (v->type) {
		case TVAL_NUM: printf("%li", v->num); break;
		case TVAL_ERR: printf("Error: %s", v->err); break;
		case TVAL_SIM: printf("%s", v->sim); break;
		case TVAL_SESPR: tval_espr_print(v, '(', ')'); break;
	}
}

void tval_println(tval* v) { tval_print(v); putchar('\n'); }

tval* tval_read_num(mpc_ast_t* t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ?
    tval_num(x) : tval_err("numero invalido");
}

tval* tval_add(tval* v, tval* x) {
	v->count++;
	v->cell = realloc(v->cell, sizeof(tval*) * v->count);
	v->cell[v->count-1] = x;
	return v;
}

tval* tval_read(mpc_ast_t* t) {

	if (strstr(t->tag, "numero")) { return tval_read_num(t); }
	if (strstr(t->tag, "simbolo")) { return tval_sim(t->contents); }

	tval* x = NULL;
	if (strcmp(t->tag, ">") == 0) { x = tval_sespr(); }
	if (strcmp(t->tag, "sespr")) { x = tval_sespr(); }

	for (int i = 0; i < t->children_num; i++) {
		if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
		if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
		if (strcmp(t->children[i]->tag, "regex") == 0) { continue; }
		x = tval_add(x, tval_read(t->children[i]));
	}

	return x;

}

/* Use operator string to see which operation to perform */
// Compiling
// tval* eval_op(tval* x, char* op, tval* y) {

// 	/* If either value is an error return it */
// 	if (x->type == TVAL_ERR) { return x; }
// 	if (y->type == TVAL_ERR) { return y; }

// 	/* Otherwise do maths on the number values */
// 	if (strcmp(op, "+") == 0) { return x->num += y->num; }
// 	if (strcmp(op, "-") == 0) { return x->num -= y->num; }
// 	if (strcmp(op, "*") == 0) { return x->num *= y->num; }
// 	if (strcmp(op, "/") == 0) {
// 	/* If second operand is zero return error */
// 	return y->num == 0
// 	  ? tval_err(TERR_DIV_ZERO)
// 	  : x->num /= y->num;
// 	}
// 	// if (strcmp(op, "%") == 0) { return x->num % y->num; }
// 	// if (strcmp(op, "^") == 0) { return pow(x->num, y->num); }

// 	return tval_err(TERR_BAD_OP);
// }

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
		/* On Success Print the AST */
		tval* x = tval_read(r.output);
		tval_println(x);
		tval_del(x);

		free(input);

	  }

	 /* Undefine and Delete our Parsers */
	mpc_cleanup(5, Numero, Simbolo, Sespr, Espr, Triestin);

	return 0;

}





