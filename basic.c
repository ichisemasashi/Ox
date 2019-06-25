#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "basic.h"

void init() {
  int size;
  size = sizeof(cell_BUF);
  memset(cell_BUF, 0x00, size);
  size = sizeof(Cons_Cell_BUF);
  memset(Cons_Cell_BUF, 0x00, size);

  size = sizeof(sym_BUF);
  memset(sym_BUF, 0x00, size);
  size = sizeof(str_BUF);
  memset(str_BUF, 0x00, size);
  size = sizeof(int_BUF);
  memset(int_BUF, 0x00, size);
  size = sizeof(float_BUF);
  memset(float_BUF, 0x00, size);
}

void free_cell(struct cell *p) {
  switch (p->kind) {
    case NIL:
      break;
    case BOOL:
      free_bool(p->value);
      break;
    case SYMBOL:
      free_symbol(p->value);
      break;
    case STRINGS:
      free_strings(p->value);
      break;
    case INT:
      free_int(p->value);
      break;
    case FLOAT:
      free_float(p->value);
      break;
    case CONS_CELL:
      free_cons_perfect(p->value);
      break;
    case empty:
    define:
      break;
  }

  p->gc = not_use;
  p->kind = empty;
  p->value = NULL;
}
void free_cons(struct cons_cell *p) {
  p->gc = not_use;
  p->car = NULL;
  p->cdr = NULL;
}

void free_cons_perfect(struct cons_cell *p) {
  if (p->car != NULL) {
    free_cell(p->car);
  }

  if (p->cdr != NULL) {
    free_cons_perfect(p->cdr);
  }

  free_cons(p);
}
void free_symbol(i_symbol *p) {
  int sz=STR_BUF_SIZE;
  memset(p->str,0x00, sz);
  p->gc = not_use;
}

void free_strings(i_strings *p) {
  int sz = STR_BUF_SIZE;
  memset(p->str, 0x00, sz);
  p->gc = not_use;
}
void free_int(i_int *p) {
  p->gc = not_use;
  p->i = 0x00;
}
void free_float(i_float *p) {
  p->gc = not_use;
  p->f = 0.00;
}
void free_bool(i_bool *p) {
  p->gc = not_use;
  p->bl = FALSE;
}
int get_cell(struct cell **p){
  int ret = NG, i;
  *p = NULL;
  for (i=0; i<Cell_BUF_SIZE; i++) {
    if (cell_BUF[i].gc == not_use) {
      cell_BUF[i].gc = use;
      *p = &(cell_BUF[i]);
      ret = OK;
      break;
    }
  }

  return ret;
}

int get_cons_cell(struct cons_cell **p) {
  int i, ret = NG;
  *p = NULL;
  for (i=0; i<Cons_Cell_BUF_SIZE; i++) {
    if (Cons_Cell_BUF[i].gc == not_use) {
      Cons_Cell_BUF[i].gc = use;
      *p = &(Cons_Cell_BUF[i]);
      ret = OK;
      break;
    }
  }

  return ret;
}
int get_prime_symbol(i_symbol **p) {
  int i, ret = NG;
  *p = NULL;
  for (i=0; i<Inner_BUF_SIZE; i++) {
    if (sym_BUF[i].gc == not_use) {
      sym_BUF[i].gc = use;
      *p = &(sym_BUF[i]);
      ret = OK;
      break;
    }
  }
  return ret;
}
int get_prime_strings(i_strings **p) {
  int i, ret = NG;
  *p = NULL;
  for (i=0; i<Inner_BUF_SIZE; i++) {
    if (str_BUF[i].gc == not_use) {
      str_BUF[i].gc = use;
      *p = &(str_BUF[i]);
      ret = OK;
      break;
    }
  }
  return ret;
}
int get_prime_int(i_int **p) {
  int i, ret = NG;
  *p = NULL;
  for (i=0; i<Inner_BUF_SIZE; i++) {
    if (int_BUF[i].gc == not_use) {
      int_BUF[i].gc = use;
      *p = &(int_BUF[i]);
      ret = OK;
      break;
    }
  }
  return ret;
}
int get_prime_float(i_float **p) {
  int i, ret = NG;
  *p = NULL;
  for (i=0; i<Inner_BUF_SIZE; i++) {
    if (float_BUF[i].gc == not_use) {
      float_BUF[i].gc = use;
      *p = &(float_BUF[i]);
      ret = OK;
      break;
    }
  }
  return ret;
}
int get_prime_bool(i_bool **p) {
  int i, ret = NG;
  *p = NULL;
  for (i=0; i<Inner_BUF_SIZE; i++) {
    if(bool_BUF[i].gc == not_use) {
      bool_BUF[i].gc = use;
      *p = &(bool_BUF[i]);
      ret = OK;
      break;
    }
  }
  return ret;
}
void set_prime_nil(struct cell *p) {
  p->kind = NIL;
  p->value = NULL;
}
int set_prime_bool(struct cell *p, bool b) {
  int ret = NG;
  i_bool *bl;

  p->kind = BOOL;
  ret = get_prime_bool(&bl);
  if (ret == NG) {
    free_cell(p);
  } else {
    bl->bl = b;
    p->value = bl;
  }
  return ret;
}

int set_prime_strings(struct cell *p, unsigned char *s, int size) {
  int ret = NG;
  i_strings *v;

  p->kind = STRINGS;
  ret = get_prime_strings(&v);
  if (ret == NG) {
    free_cell(p);
  } else if (STR_BUF_SIZE < size) {
    free_cell(p);
  } else {
    memcpy(v->str, &(s[1]), (size - 2));
    p->value = v;
  }
  return ret;
}
int set_prime_symbol(struct cell *p, unsigned char *s, int size) {
  int ret = NG;
  i_symbol *v;

  p->kind = SYMBOL;
  ret = get_prime_symbol(&v);
  if (ret == NG) {
    free_cell(p);
  } else if (STR_BUF_SIZE < size) {
    free_cell(p);
  } else {
    memcpy(v->str, s, size);
    p->value = v;
  }
  return ret;
}
int set_prime_int(struct cell *p, int i) {
  int ret = NG;
  i_int *v;

  p->kind = INT;
  ret = get_prime_int(&v);
  if (ret == NG) {
    free_cell(p);
  } else {
    v->i = i;
    p->value = v;
  }
  return ret;
}
int set_prime_float(struct cell *p, double f) {
  int ret = NG;
  i_float *v;

  p->kind = FLOAT;
  ret = get_prime_float(&v);
  if (ret == NG) {
    free_cell(p);
  } else {
    v->f = f;
    p->value = v;
  }
  return ret;
}

int is_parce_int(unsigned char *str, int size) {
  int ret = NG, i;
  char c;

  if (8 < size) {
      /* MAX 8ケタにしておく */
  } else if ((is_my_sign((char)str[0]) == OK) || (isdigit(str[0]))) {
    /* 先頭が+/- 残りが数字 */
    c = (char)str[1];
    if (c == '0') {
      if ((str[1] == 'x') || (str[1] == 'X')) {
        for(i=2;(i<size) && (is_my_hex((char)str[i])); i++) {}
        if (i == size) {
          /* 0x... */
          ret = OK;
        }
      } else if ((str[1] == 'b') || (str[1] == 'B')) {
        for(i=2;(i<size) && (is_my_bin((char)str[i])); i++) {}
        if (i == size) {
          /* 0b... */
          ret = OK;
        }
      }
    } else if (isdigit(c)) {
      for(i=1;(i<size) && (isdigit((char)str[i]));i++) { }
      if (i == size) {
        /* 先頭0以外で、のこり全部数字 */
        ret = OK;
      }
    }
  }
  return ret;
}
void prompter(char *str) {printf("%s ",str);}
int read_from(FILE *f, unsigned char *out, int sz) {
  int paren = 0, c, ret = OK, i = 0;

  while((c = getc(f)) != EOF) {
    if (c == '\n') {
      if (paren == 0) {
        ret = OK;
        break;
      }
      continue;
    }

    if (c == '(') {
      paren++;
    } else if (c == ')') {
      paren--;
    }
    if (sz <= i) {
      ret = NG;
      break;
    }
    out[i] = (unsigned char)c;
    i++;
  }

  if (ret == OK) {
    out[i] = 0x00;
  }
  return ret;
}
int make_cons_cells_list(struct cell *parent, unsigned char *str) {
  int ret = NG, i = 0;
  struct cons_cell *p, *n;
  struct cell *c_p;

  ret = get_cons_cell(&p);
  if (ret == NG) {
    printf("[DBG] make_cons_cells_list() get_cons_cell NG.\n");
    free_cell(parent);
    return ret;
  }
  parent->kind = CONS_CELL;
  parent->value = p;
  i++;

  while((str[i] != 0x00) && (str[i] != ')')) {
    i += skip_spaces(str);
    if (str[i] == '(') {
      ret = get_cell(&c_p);
      if (ret == NG) {
        free_cell(parent);
        return NG;
      }
      ret = make_cons_cells_list(c_p, &(str[i+1]));
    } else {
      ret = get_cell(&c_p);
      if (ret == NG) {
        free_cell(parent);
        return ret;
      }
      ret = make_cells_from_parce_strings(c_p, &(str[i]));
      if (ret == NG) {
        free_cell(parent);
        return ret;
      }
      p->car = c_p;
      ret = get_cons_cell(&n);
      if (ret == NG) {
        free_cell(parent);
        return NG;
      }
      p->cdr = n;
      p = n;
      i += my_sizeof(&(str[i]));
    }
  }

  /* )の場合  */
  if (str[i] == ')') {
    p->cdr = NULL;
  }
  return ret;
}
int make_cons_from_parce_strings(unsigned char *str, int size) {
  int i = 0, ret = NG;
  struct cons_cell *cons_p;
  struct cell *cell_p;

  while (str[i] != 0x00) {
    if (str[i] == '(') {
      ret = make_cons_cells_list(&my_cell, str);
    }
    i++;
  }
  return ret;
}
int my_sizeof(unsigned char *p) {
  int i;
  for(i=0;*p != 0x00;i++,p++) {
    if (isspace(p[i])) {
      i--;
      break;
    }
  }
  return i;
}
int is_my_hex(char c) {
  int ret = NG;
  if ((isdigit(c)) || (('a' <= c) && (c <= 'f')) ||
      (('A' <= c) && (c <= 'F'))) {
    ret = OK;
  }
  return ret;
}
int is_my_bin(char c) {
  int ret = NG;
  if((c == '0') || (c == '1')) {
    ret = OK;
  }
  return ret;
}
int my_atoi(unsigned char *str, int size) {
  int ret = 0, sign = 1, i=0;

  /* +/- */
  if (is_my_sign((char)str[0]) == OK){
      i++;
  }

  if (str[0] == '-') {
    sign = -1;
  }

  if ((str[i] == '0') && ((str[i+1] == 'b') || (str[i+1] == 'B')) ) {
    /* 2進数 */
    for(i=i+2;i<size;i++) {
      ret = (str[i] - '0') + 2*ret;
    }
  } else if ((str[i] == '0') && ((str[i+1] == 'x') || (str[i+1] == 'X'))) {
    /* 16進数 */
    for(i=i+2;i<size;i++) {
      if (isnumber(str[i])) {
        ret = (str[i] - '0') + 16 * ret;
      } else if (('a' <= str[i]) && (str[i] <= 'f')) {
        ret = (str[i] - 'a' + 10) + 16 * ret;
      } else {
        ret = (str[i] - 'A' + 10) + 16 * ret;
      }
    }
  } else {
    /* 10進数 */
    for(;i<size;i++) {
      ret = (str[i] - '0') + 10 * ret;
    }
  }

  ret = ret * sign;

  return ret;
}
int is_my_sign(char c) {
  int ret = NG;
  if ((c == '+') || (c == '-')) {
    ret = OK;
  }
  return ret;
}

/* -------------------------------------------*/
/*  今回は10進の小数に限定する。              */
/* -------------------------------------------*/
int is_parce_float(unsigned char *s, int size) {
  int ret = NG, pnt_cnt = 0, i = 0;

  if (STR_BUF_SIZE<size) {
    return ret;
  }

  if (is_my_sign((char)s[0]) == OK) {
    i++;
  }

  for(;i<size;i++) {
    if (s[i] == '.') {
      pnt_cnt++;
      if (1<pnt_cnt) {
        break;
      }
    } else if (!isdigit((char)s[i])) {
      break;
    }
  }

  if (i == size) {
    ret = OK;
  }
  return ret;
}
double  my_atod(unsigned char *s, int size) {
  double d;
  char str[STR_BUF_SIZE], *endptr;

  memcpy(str, s, size);
  str[size] = '\0';

  d = strtod(str, &endptr);
  return d;
}

int make_cells_from_parce_strings(struct cell *cell_p, unsigned char *str) {
  int i,c ,sz , ret = NG;
  double f;

  sz = my_sizeof(str);
  if (STR_BUF_SIZE < sz) {
    ret = NG;
  }  else {
    cell_p->gc = use;
    if ((sz == 3) &&
        ((strncmp((char *)str, "NIL", 3) == 0) ||
         (strncmp((char *)str, "nil", 3) == 0))) {
      /* NIL */
      set_prime_nil(cell_p);
      ret = OK;
    } else if ((sz == 5) &&
               ((strncmp((char *)str, "FALSE", 5) == 0) ||
                (strncmp((char *)str, "false", 5) == 0))) {
      /* BOOL(FALSE) */
      ret = set_prime_bool(cell_p, FALSE);
    } else if ((sz == 4) &&
               ((strncmp((char *)str, "TRUE", 4) == 0) ||
                (strncmp((char *)str, "true", 4) == 0))) {
      /* BOOL(TRUE) */
      ret = set_prime_bool(cell_p, TRUE);
    } else if (str[0] == '"') {
      if (str[sz-1] == '"') {
        /* STRINGS */
        ret = set_prime_strings(cell_p, str, sz);
      }
    } else if (is_parce_int(str, sz) == OK) {
      /* INT */
      i = my_atoi(str, sz);
      ret = set_prime_int(cell_p, i);
    } else if (is_parce_float(str, sz) == OK) {
      /* FLOAT */
      f = my_atod(str, sz);
      ret = set_prime_float(cell_p, f);
    } else {
      /* SYMBOL */
      ret = set_prime_symbol(cell_p, str, sz);
    }
  }
  return ret;
}
int skip_spaces(unsigned char *p) {
  int i;
  for (i=0;(*p!=0x00) && (isspace(p[i]));i++) {
  }
  return i;
}

int parse_input_1_sexp(unsigned char *in, int sz) {
  int ret = NG, i=0;

  i = skip_spaces(in);

  if (in[i] == '(') {
    /* paren start */
    ret = make_cons_from_parce_strings(&(in[i]), sz);
  } else {
    /* simple symbole or something */
    ret = make_cells_from_parce_strings(&my_cell, &(in[i]));
  }
  return ret;
}

void copy_cell(struct cell *from, struct cell *to) {
  to->gc = from->gc;
  to->kind = from->kind;
  to->value = from->value;
}
int eval_cons_cell(struct cell *p, struct cell *result) {
  return OK;
}
void print_cell(struct cell *p) {
  switch (p->kind) {
    case NIL:
      printf("NIL\n");
      break;
    case SYMBOL:
      printf("%s\n", ((i_symbol *)(p->value))->str);
      break;
    case STRINGS:
      printf("\"%s\"\n", ((i_strings *)(p->value))->str);
      break;
    case INT:
      printf("%d\n", ((i_int *)(p->value))->i);
      break;
    case FLOAT:
      printf("%f\n", ((i_float *)(p->value))->f);
      break;
    case BOOL:
      if (((i_bool *)(p->value))->bl == TRUE) {
        printf("TRUE\n");
      } else {
        printf("FALSE\n");
      }
      break;
    case empty:
    case CONS_CELL:
    default:
      break;
  }
  free_cell(p);
}
void print_cons(struct cell *p) {
}

int eval_cons(struct cell *p, struct cell *result) {
  int ret = NG;
  switch (p->kind) {
    case empty:
      break;
    case NIL:
    case SYMBOL:
    case STRINGS:
    case INT:
    case FLOAT:
    case BOOL:
      ret = OK;
      copy_cell(p, result);
      break;
    case CONS_CELL:
      ret = eval_cons_cell(p, result);
      break;
    default:
      break;
  }
  return ret;
}

int my_read() {
  int ret, size;
  unsigned char InputBuf[4096];

  size = sizeof(InputBuf)/sizeof(unsigned char);
  prompter("Lisp >");
  ret = read_from (stdin, InputBuf, size);
  if (ret == NG) {
    return NG;
  }
  ret = parse_input_1_sexp(InputBuf, size);
  return ret;

}
int my_eval() {
  int ret = NG;

  if (my_cell.kind != empty) {
    ret = eval_cons(&my_cell, &my_printer_cell);
  }
  return ret;
}

int my_print() {
  if (my_printer_cell.kind == empty) {
    return NG;
  } else if (my_printer_cell.kind == CONS_CELL) {
    print_cons(&my_printer_cell);
  } else {
    print_cell(&my_printer_cell);
  }
  return OK;
}

int main () {
    int ret;

    init();
    while(1) {
        ret = my_read();
        if(ret == NG) {
            return -1;
        }
        ret = my_eval();
        if (ret == NG) {
            return -1;
        }
        ret = my_print();
        if (ret == NG) {
            return -1;
        }
    }
}
