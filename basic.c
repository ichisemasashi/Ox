#include <stdio.h>
#include <string.h>
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
  }

  if (p->cdr != NULL) {

  }

  free_cons(p);

void free_symbol(i_symbol *);
void free_strings(i_strings *);
void free_int(i_int *);
void free_float(i_float *);
int get_cell(struct cell *p){
  int ret = NG, i;

  for (i=0; i<Cell_BUF_SIZE; i++) {
    if (cell_BUF[i].gc == not_use) {
      p = &(cell_BUF[i]);
      ret = OK;
      break;
    }
  }

  return ret;
}
int get_cons_cell(struct cons_cell *p) {
  int i, ret = NG;

  for (i=0; i<Cons_Cell_BUF_SIZE; i++) {
    if (Cons_Cell_BUF[i].gc == not_use) {
      p = &(Cons_Cell_BUF[i]);
      ret = OK;
      break;
    }
  }

  return ret;
}
int get_symbol(i_symbol *);
int get_strings(i_strings *);
int get_int(i_int *);
int get_float(i_float *);

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
int parse_input(unsigned char *in, int sz) {
  int ret = OK;
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
  ret = parse_input(InputBuf, size);
  return OK;

}
int my_eval() {
  return OK;
}
int my_print() {
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
