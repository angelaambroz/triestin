// cc -std=c99 -Wall trst.c mpc.c -ledit -lm -o trst

#include "mpc.h"

#ifdef _WIN32

static char buffer[2048];

char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

void add_history(char* unused) {}

#else
#include <editline/readline.h>
// #include <editline/history.h>
#endif

#define TASSERT(args, cond, err) if (!(cond)) { tval_del(args); return tval_err(err); }

// Forward declarations
struct tval;
struct tenv;
typedef struct tval tval;
typedef struct tenv tenv;

enum { TVAL_ERR, TVAL_NUM, TVAL_SIM, 
    TVAL_FUN, TVAL_SESPR, TVAL_QESPR };

typedef struct tval {
  int type;
  // Just a plain ol' number
  long num;
  /* Error and Simbolo types have some string data */
  char* err;
  char* sim;
  /* Count and Pointer to a list of "tval*"; */
  int count;
  struct tval** cell;
} tval;


tval* tval_eval(tval* v);
tval* tval_aderire(tval* x, tval* y);
void tval_print(tval* v);

/* Construct a pointer to a new Numero tval */ 
tval* tval_num(long x) {
  tval* v = malloc(sizeof(tval));
  v->type = TVAL_NUM;
  v->num = x;
  return v;
}

/* Construct a pointer to a new Error tval */ 
tval* tval_err(char* m) {
  tval* v = malloc(sizeof(tval));
  v->type = TVAL_ERR;
  v->err = malloc(strlen(m) + 1);
  strcpy(v->err, m);
  return v;
}

/* Construct a pointer to a new Simbolo tval */ 
tval* tval_sim(char* s) {
  tval* v = malloc(sizeof(tval));
  v->type = TVAL_SIM;
  v->sim = malloc(strlen(s) + 1);
  strcpy(v->sim, s);
  return v;
}

/* A pointer to a new empty Sespr tval */
tval* tval_sespr(void) {
  tval* v = malloc(sizeof(tval));
  v->type = TVAL_SESPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

tval* tval_qespr(void) {
  tval* v = malloc(sizeof(tval));
  v->type = TVAL_QESPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

void tval_del(tval* v) {

  switch (v->type) {
    /* Do nothing special for numero type */
    case TVAL_NUM: break;
    
    /* For Err or Sym free the string data */
    case TVAL_ERR: free(v->err); break;
    case TVAL_SIM: free(v->sim); break;
    
    /* If Sespr then delete all elements inside */
    case TVAL_SESPR:
    case TVAL_QESPR:
      for (int i = 0; i < v->count; i++) {
        tval_del(v->cell[i]);
      }
      /* Also free the memory allocated to contain the pointers */
      free(v->cell);
    break;
  }
  
  /* Free the memory allocated for the "tval" struct itself */
  free(v);
}

tval* tval_add(tval* v, tval* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(tval*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}

tval* tval_pop(tval* v, int i) {
  /* Find the item at "i" */
  tval* x = v->cell[i];
  
  /* Shift memory after the item at "i" over the top */
  memmove(&v->cell[i], &v->cell[i+1],
    sizeof(tval*) * (v->count-i-1));
  
  /* Decrease the count of items in the list */
  v->count--;
  
  /* Reallocate the memory used */
  v->cell = realloc(v->cell, sizeof(tval*) * v->count);
  return x;
}

tval* tval_spinta(tval* x, tval* y) {
  tval_print(x);
  printf("\n");
  tval_print(y);
  printf("\n");
  printf("The x count is %d\n", x->count);
  printf("The first cell is ");
  tval_print(x->cell[0]);
  printf("\n");

  // Lengthen mem space
  x->cell = realloc(x->cell, sizeof(tval*) * x->count+1);

  // Shift everything down by 1
  for (int i = 0; i < x->count; i++) {
    memmove(&x->cell[i+1], &x->cell[i], sizeof(tval*));
  }

  // Add y thing to cell
  x->cell[0] = y;

  // Iterate counter
  x->count++;

  // Make it all a qespr
  x->type = TVAL_QESPR;

  return x;

}

tval* tval_take(tval* v, int i) {
  tval* x = tval_pop(v, i);
  tval_del(v);
  return x;
}

void tval_espr_print(tval* v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {
    
    /* Print Value contained within */
    tval_print(v->cell[i]);
    
    /* Don't print trailing space if last element */
    if (i != (v->count-1)) {
      putchar(' ');
    }
  }
  putchar(close);
}

void tval_print(tval* v) {
  switch (v->type) {
    case TVAL_NUM:   printf("%li", v->num); break;
    case TVAL_ERR:   printf("Error: %s", v->err); break;
    case TVAL_SIM:   printf("%s", v->sim); break;
    case TVAL_SESPR: tval_espr_print(v, '(', ')'); break;
    case TVAL_QESPR: tval_espr_print(v, '{', '}'); break;
  }
}

void tval_println(tval* v) { tval_print(v); putchar('\n'); }

tval* builtin_op(tval* a, char* op) {
  
  /* Ensure all arguments are numeros */
  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != TVAL_NUM) {
      tval_del(a);
      return tval_err("Non si puo operare su un non-numero.");
    }
  }
  
  /* Pop the first element */
  tval* x = tval_pop(a, 0);
  
  /* If no arguments and sub then perform unary negation */
  if ((strcmp(op, "-") == 0) && a->count == 0) {
    x->num = -x->num;
  }
  
  /* While there are still elements remaining */
  while (a->count > 0) {
  
    /* Pop the next element */
    tval* y = tval_pop(a, 0);
    
    /* Perform operation */
    if (strcmp(op, "+") == 0) { x->num += y->num; }
    if (strcmp(op, "-") == 0) { x->num -= y->num; }
    if (strcmp(op, "*") == 0) { x->num *= y->num; }
    if (strcmp(op, "%") == 0) { x->num = x->num % y->num; }
    if (strcmp(op, "/") == 0) {
      if (y->num == 0) {
        tval_del(x); tval_del(y);
        x = tval_err("Divisione per zero.");
        break;
      }
      x->num /= y->num;
    }
    
    /* Delete element now finished with */
    tval_del(y);
  }
  
  /* Delete input espression and return result */
  tval_del(a);
  return x;
}

tval* builtin_testa(tval* a) {
  TASSERT(a, a->count == 1, "`testa` ha troppi argomenti.");
  TASSERT(a, a->cell[0]->type == TVAL_QESPR, "`testa` ha passato tipi sbagliati.");
  TASSERT(a, a->cell[0]->count != 0, "`testa` ha passato {}.");
  tval* v = tval_take(a, 0);
  while (v->count > 1) { tval_del(tval_pop(v, 1)); }
  return v;
}

tval* builtin_lung(tval* a) {
  TASSERT(a, a->count == 1, "`lung` ha troppi argomenti.");
  TASSERT(a, a->cell[0]->type == TVAL_QESPR, "`lung` ha passato tipi sbagliati.");
  return tval_num(a->cell[0]->count);
}

tval* builtin_coda(tval* a) {
  TASSERT(a, a->count == 1, "`coda` ha troppi argomenti.");
  TASSERT(a, a->cell[0]->type == TVAL_QESPR, "`coda` ha passato tipi sbagliati.");
  TASSERT(a, a->cell[0]->count != 0, "`coda` ha passato {}.");
  tval* v = tval_take(a, 0);
  tval_del(tval_pop(v, 0));
  return v;
}

tval* builtin_lista(tval* a) {
  a->type = TVAL_QESPR;
  return a;
}

tval* builtin_valu(tval* a) {
  TASSERT(a, a->count == 1, "`valu` ha troppi argomenti.");
  TASSERT(a, a->cell[0]->type == TVAL_QESPR, "`valu` ha passato tipi sbagliati.");
  tval* x = tval_take(a, 0);
  x->type = TVAL_SESPR;
  return tval_eval(x);
}

tval* builtin_aderire(tval* a) {
  for (int i = 0; i < a->count; i++) {
    TASSERT(a, a->cell[i]->type == TVAL_QESPR, "`aderire` ha passato tipi sbagliati.");
  }

  tval* x = tval_pop(a, 0);

  while (a->count) {
    x = tval_aderire(x, tval_pop(a, 0));
  }

  tval_del(a);
  return x;
}

tval* builtin_spinta(tval* a) {
  // TASSERT(a, a->count == 0, "`spinta` non ha abbastanza argomenti.");
  TASSERT(a, a->cell[0]->type == TVAL_NUM, "`spinta` accetta un numero e un qespr.");
  TASSERT(a, a->cell[1]->type == TVAL_QESPR, "`spinta` accetta un numero e un qespr.");
  tval* x = tval_pop(a, 0);
  return tval_spinta(a, x);
}


tval* builtin(tval* a, char* func) {
  if (strcmp("lista", func) == 0) { return builtin_lista(a); }
  if (strcmp("testa", func) == 0) { return builtin_testa(a); }
  if (strcmp("coda", func) == 0) { return builtin_coda(a); }
  if (strcmp("aderire", func) == 0) { return builtin_aderire(a); }
  if (strcmp("valu", func) == 0) { return builtin_valu(a); }
  if (strcmp("lung", func) == 0) { return builtin_lung(a); }
  if (strcmp("spinta", func) == 0) { return builtin_spinta(a); }
  if (strcmp("+-/*", func) == 0) { return builtin_op(a, func); }
  tval_del(a);
  return tval_err("Cosa?");
}


tval* tval_aderire(tval* x, tval* y) {
  while (y->count) {
    x = tval_add(x, tval_pop(y, 0));
  }
  tval_del(y);
  return x;
}

tval* tval_eval_sespr(tval* v) {
  
  /* Evaluate Children */
  for (int i = 0; i < v->count; i++) {
    v->cell[i] = tval_eval(v->cell[i]);
  }
  
  /* Error Checking */
  for (int i = 0; i < v->count; i++) {
    if (v->cell[i]->type == TVAL_ERR) { return tval_take(v, i); }
  }
  
  /* Empty Expression */
  if (v->count == 0) { return v; }
  
  /* Single Expression */
  if (v->count == 1) { return tval_take(v, 0); }
  
  /* Ensure First Element is Simbolo */
  tval* f = tval_pop(v, 0);
  if (f->type != TVAL_SIM) {
    tval_del(f); tval_del(v);
    return tval_err("S-espresione non inizia con un simbolo.");
  }
  
  /* Call builtin with operator */
  tval* result = builtin(v, f->sim);
  tval_del(f);
  return result;
}

tval* tval_eval(tval* v) {
  /* Evaluate Sespressions */
  if (v->type == TVAL_SESPR) { return tval_eval_sespr(v); }
  /* All other tval types remain the same */
  return v;
}

tval* tval_read_num(mpc_ast_t* t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ?
    tval_num(x) : tval_err("numero invalido");
}

tval* tval_read(mpc_ast_t* t) {
  
  /* If Simbolo or Numero return conversion to that type */
  if (strstr(t->tag, "numero")) { return tval_read_num(t); }
  if (strstr(t->tag, "simbolo")) { return tval_sim(t->contents); }
  
  /* If root (>) or sespr then create empty list */
  tval* x = NULL;
  if (strcmp(t->tag, ">") == 0) { x = tval_sespr(); } 
  if (strstr(t->tag, "sespr"))  { x = tval_sespr(); }
  if (strstr(t->tag, "qespr"))  { x = tval_qespr(); }
  
  /* Fill this list with any valid espression contained within */
  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
    if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "{") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "}") == 0) { continue; }
    if (strcmp(t->children[i]->tag,  "regex") == 0) { continue; }
    x = tval_add(x, tval_read(t->children[i]));
  }
  
  return x;
}

int main(int argc, char** argv) {
  
  mpc_parser_t* Numero = mpc_new("numero");
  mpc_parser_t* Simbolo = mpc_new("simbolo");
  mpc_parser_t* Sespr  = mpc_new("sespr");
  mpc_parser_t* Qespr  = mpc_new("qespr");
  mpc_parser_t* Espr   = mpc_new("espr");
  mpc_parser_t* Triestin  = mpc_new("triestin");
  
  mpca_lang(MPCA_LANG_DEFAULT,
    "                                          \
      numero : /-?[0-9]+/ ;                    \
      simbolo : \"lista\" | \"testa\" | \"coda\" | \"valu\" \
              | \"aderire\" | \"lung\" | \"spinta\" \
              | '+' | '-' | '*' | '/' | '%' ;  \
      sespr  : '(' <espr>* ')' ;               \
      qespr  : '{' <espr>* '}' ;               \
      espr   : <numero> | <simbolo> | <sespr> | <qespr> ; \
      triestin  : /^/ <espr>* /$/ ;               \
    ",
    Numero, Simbolo, Sespr, Qespr, Espr, Triestin);
  
  puts("Triestin Version 0.0.0.0.10");
  puts("Creato con <3");
  puts("Per uscire, ctrl+C\n");
  
  while (1) {
  
    char* input = readline("triestin> ");
    add_history(input);
    
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Triestin, &r)) {
      tval* x = tval_eval(tval_read(r.output));
      tval_println(x);
      tval_del(x);
      mpc_ast_delete(r.output);
    } else {
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    
    free(input);
    
  }
  
  mpc_cleanup(6, Numero, Simbolo, Sespr, Qespr, Espr, Triestin);
  
  return 0;
}
