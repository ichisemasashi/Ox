#include <stdio.h>
#include <float.h>

#define MAXBUF 4096 /* buf size */
#define MAXTOKEN 4096 /* token size */
#define MAXMEMORY 4096
#define MAXSTRINGS 20

#define TRUE 0
#define FALSE 1

static int buf[MAXBUF];
static int memBuf[MAXBUF];
static struct memory {
    int* symbol;
    int* val;
    struct memory* nextp;
} memoris[MAXMEMORY];
static struct memory* top_memory;

static struct token{
   int* tokenp;
   int size;
   struct token* nextp;
} tokens[MAXTOKEN];

enum useflag { use, not_use};
enum typeflag { INT, FLOAT, STRING, CONS, SYMBOL, NIL};

struct Data {
    enum typeflag typeflag;
    enum useflag useflag;
    int int_data;
    float float_data;
    char char_data[MAXSTRINGS];
    struct Cons * cons;
};

struct Cons {
    struct Data * car; /* car */
    struct Cons * cdr; /* cdr */
    enum useflag useflag;
};

struct Cons ConsCells[MAXBUF];
struct Data Datas[MAXBUF];
int index_of_Consceslls;
int index_of_Datas;

/* prot type */
int isNumber(struct token*);
int isFloat (struct token*);
int isString (struct token*);
int copyString(struct token *, struct Data *);
int getData ();
int isParlen (struct token* );
int isParlenEnd(struct token*);
int isParlenStart(struct token*);
float toFloat(int *, int);
int toInt(struct token *);
int getConsCell();

void putToken() {
    int i,j;
    for (i=0;tokens[i].nextp!=NULL;i++) {
        for (j=0;j<tokens[i].size;j++) {
            putc(tokens[i].tokenp[j],stdout);
        }
        putc('\n',stdout);
    }
}
int getBuf (int s[], int limit) {
    int c,i,p;
    for (i=0,p=0;(i<limit) && ((c=getchar())!=EOF) ;i++) {
        s[i] = c;
        if (c == '(') { /* ( */
            p++;
        } else if (c == ')') { /* ) */
            p--;
        } else if (c == '\n') {
            if (p == 0) {
                break;
            }
        }
    }
    s[i] = '\0';
    return i;
}
int tokenize (int in[], int limit_in, struct token out[], int limit_out) {
    int c,i,o,t;
    enum mode {IN,OUT};
    enum mode m;
    for (i=0,o=0,t=0;(i<limit_in) && (o<limit_out) && (in[i]!='\0');i++,t++) {
        c = in[i];
        if (c==' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f' || c == '\r') {
           /* white space */
            if (m == IN) {
                m = OUT;
                out[o].size = t;
                out[o].nextp = &out[o+1];
                o++;
            }
        } else if (c == '(') { /* ( */
            if (m == IN) {
                m = OUT;
                out[o].size = t;
                out[o].nextp = &out[o+1];
                o++;
            } else {
            }
            out[o].tokenp = &in[i];
            out[o].size = 1;
            out[o].nextp = &out[o+1];
            o++;
            m = OUT;
        } else if (c==')') { /* ) */
            if (m == IN) {
                m = OUT;
                out[o].size = t;
                out[o].nextp = &out[o+1];
                o++;
            } else {
            }
            out[o].tokenp = &in[i];
            out[o].size = 1;
            out[o].nextp = &out[o+1];
            o++;
            m = OUT;
        } else { /* words */
            if (m == OUT) {
                m = IN;
                t = 0;
                out[o].tokenp = &in[i];
            }
        }
    }
    if (in[i] == '\0') {
        out[o].nextp = NULL;
    }
    return o;
}
int readAtom (struct token* in, struct Data *data) {
    int result;
    if (isNumber(in) == TRUE) {
        if (isFloat (in) == TRUE) {
            data->typeflag = FLOAT;
            data->float_data = toFloat(in->tokenp,in->size);
        } else { /* Int */
            data->typeflag = INT;
            data->int_data = toInt(in);
        }
    } else if (isString(in) == TRUE) {
        data->typeflag = STRING;
        result = copyString(in,data);
        if (result == FALSE) {
            return FALSE;
        }
    } else { /* Symbol */
        data->typeflag = SYMBOL;
        result = copyString (in, data);
        if (result == FALSE) {
          return FALSE;
        }
    }
    return TRUE;
}

int copyString(struct token *from, struct Data *to) {
    int i;

    if (from->size >=MAXSTRINGS) {
        return FALSE;
    }

    for (i=0;i<from->size;i++) {
        to->char_data[i] = (char) from->tokenp[i];
    }

    return TRUE;
}

int readS (struct token *from, struct Data *to) {
    int i,result;
    struct Cons *nextCell;

    to->typeflag =CONS;
    i = getConsCell();
    if (i == MAXBUF) {
        return FALSE;
    }
    to->cons = &ConsCells[i];
    nextCell = &ConsCells[i];

    if (isParlenEnd (from->nextp) == TRUE) {
      from = from->nextp;
      /* '() is as a atom */
      i = getData();
      if (i == MAXBUF) {
        return FALSE;
      }
      Datas[i].useflag = use;
      Datas[i].typeflag = NIL;
      nextCell->car = &Datas[i];
      i = getConsCell();
      if (i == MAXBUF) {
        return FALSE;
      }
      nextCell->cdr = &ConsCells[i];
      nextCell = &ConsCells[i];
      nextCell->useflag = use;
      i = getData();
      if (i == MAXBUF) {
        return FALSE;
      }
      to = &Datas[i];

      return TRUE;
    }

    while (from->nextp != NULL) {
        from = from->nextp;
        if (isParlenEnd (from) == TRUE) {
            /* end of S-exp */
        } else if ((isParlenStart (from) == TRUE) && (isParlenEnd (from->nextp) == TRUE )) {
            /* '() is as a atom */
            i = getData ();
            if (i==MAXBUF) {
                return FALSE;
            }
            Datas[i].useflag = use;
            Datas[i].typeflag = NIL;
            nextCell->car =&Datas[i];
            i = getConsCell ();
            if (i == MAXBUF) {
                return FALSE;
            }
            nextCell->cdr = &ConsCells[i];
            nextCell = &ConsCells[i];
                        nextCell->useflag = use;

        } else if (isParlenStart (from) == TRUE) {
            /* start of S-exp */
            i = getData ();
            if (i == MAXBUF) {
                return FALSE;
            }
            to = &Datas[i];
            readS (from, to);
        }
    }
    return TRUE;
}

int myRead () {
    int i;
    i = getBuf (buf,MAXBUF);
    if (i > MAXBUF) {
        return FALSE;
    }
    i = tokenize (buf, MAXBUF, tokens, MAXTOKEN);
    if (i > MAXTOKEN) {
        return FALSE;
    }
    i = getData();
    if (i == MAXBUF) {
      return FALSE;
    }
    Datas[i].useflag = use;
    /* S式ならreadS */
    /* S式でないならreadAtom */
    if (isParlen(&tokens[0]) == TRUE) {
        readS(&tokens[0], &Datas[i]);
    } else {
      readAtom (&tokens[0],&Datas[i]);
    }

    return TRUE;
}
int isParlen (struct token* p) {
    int result = FALSE;
    if (p->size == 1) {
        if ( (p->tokenp[0] == '(') || (p->tokenp[0] == ')') ) {
            result = TRUE;
        }
    }
    return result;
}
int isParlenStart (struct token* p) {
    if (p->size == 1) {
        if (p->tokenp[0] == '(') {
            return TRUE;
        }
    }
    return FALSE;
}
int isParlenEnd (struct token *p) {
  if (p->size == 1) {
    if (p->tokenp[0] == ')') {
      return TRUE;
    }
  }
  return FALSE;
}

int isNumber (struct token* x) {
    int i;
    for (i = 0;i < x->size;i++) {
        if (('0' <= x->tokenp[i]) && (x->tokenp[i] <= '9')) { /* number */
        } else if ((x->tokenp[i] == '+') || (x->tokenp[i] == '-')) { /* +- */
        } else if (x->tokenp[i] == '.') { /* . */
        } else {
            return FALSE;
        }
    }
    return TRUE;
}
int isFloat (struct token* x) {
    int i;
    int flag = FALSE;
    for (i=0;i < x->size;i++) {
        if (x->tokenp[i] == '.') {
            flag = TRUE;
            break;
        }
    }
    return flag;
}

int isString (struct token* x) {
    int result;
    if ( (x->tokenp[0] == '"') && (x->tokenp[x->size - 1] == '"')) {
        result = TRUE;
    } else {
        result = FALSE;
    }
    return result;
}
int isSymbol (struct token* x) {
    if ((x->tokenp[0] == '(') || (x->tokenp[0] == ')')) {
        return FALSE;
    }
    if (isNumber (x)) {
        return FALSE;
    }
    return TRUE;
}
int isLiteral (struct token* x) {
    if ((x->tokenp[0] == '(') || (x->tokenp[0] == ')')) {
        return FALSE;
    }
    return isNumber (x);
}
struct memory* findSymbol (struct token* t) {
    int i;
    struct memory* m;

    for (m = top_memory;m->nextp != NULL;m = m->nextp) {
        for (i = 0; i < t->size; i++) {
            if (t->tokenp[i] != m->symbol[i]) {
                break;
            }
        }
        if (i == t->size) {
            return m;
        }
    }
    return NULL;
}
float myPow (int x) {
    int i;
    float ret = 1.0;
    if (x != 0) {
        for (i = 1;i <= x;i++) {
            ret = ret * 0.1;
        }
    }
    return ret;
}
int toInt(struct token *p) {
    int result, i;
    for(i=0;i<p->size;i++) {
        result = p->tokenp[i] - '0';
        if (i < p->size - 1) {
            result = result * 10;
        }
    }
    return result;
}

float toFloat(int *p, int size) {
    float ret,dec;
    int i,c,n = 0;
    for (i = 0,ret = 1;i < size;i++) {
        c = p[i];
        if (c == '-') { /* +- */
            ret = ret * -1;
        } else if (c == '.') { /* . */
            n = 1;
        } else if ('0' <= c && c <= '9') { /* 0~9 */
            ret = (ret * 10) + (c - '0');
            if (n != 0) {
                n++;
            }
        }
    }
    if (n > 0) {
        ret = ret * myPow (n);
    }
    return ret;
}

int mySize(int *x) {
    int i;
    for (i = 0;x[i] != '\0';i++) {;}
    return i-1;
}
int isQuote (struct token* x) {
    int result = FALSE;
    if (x->size == 5) {
        if ((x->tokenp[0] == 'q') &&
            (x->tokenp[1] == 'u') &&
            (x->tokenp[2] == 'o') &&
            (x->tokenp[3] == 't') &&
            (x->tokenp[4] == 'e')) {
            result = TRUE;
        }
    }
    return result;
}
void initCells() {
    int i;
    for (i=0;i<MAXBUF;i++) {
      ConsCells[i].useflag = not_use;
      Datas[i].useflag = not_use;
    }
    index_of_Consceslls = 0;
    index_of_Datas = 0;
}
int getConsCell () {
  int i,loop;
  for (loop = 0,i=index_of_Consceslls;i<MAXBUF;i++) {
    if (ConsCells[i].useflag == not_use) {
      break;
    }
    if ((loop == 0) && (i == (MAXBUF - 1))) {
      loop++;
      i = 0;
      continue;
    }

  }
  index_of_Consceslls = i;
  return i;
}
int getData () {
    int i,loop;
    for (loop = 0,i = index_of_Datas;i< MAXBUF; i++) {
        if (i == MAXBUF -1) {
          loop++;
          i = 0;
          continue;
        }
        if (Datas[i].useflag == not_use) {
            break;
        }
    }
    index_of_Datas = i;
    return i;
}

float Eval () {
    struct token* x = tokens;
    struct memory* m;
    /* 変数参照 */
    if (isSymbol (x) == TRUE) {
       m = findSymbol (x);
       return (toFloat (m->val, mySize(m->val)));
    /* 定数リテラル */
    } else if (isLiteral (x) == TRUE) {
        return (toFloat (x->tokenp, x->size));
    /* (quote exp) */
    } else if (isQuote (x->nextp) == TRUE) {
        /* quoted list of x */
    }
    return FLT_MAX;
    /* (if test conseq alt) */
    /* (set! var exp) */
    /* (define var exp) */
    /* (lambda (var*) exp) */
    /* (begin exp*) */
    /* (proc exp*) */
}
void myEval () {
}
void myPrint () {
}
int main () {
    int ret;
    printf("my version lispy...\n");
    initCells();

    while (1) {
        printf ("Lispy >");
        ret = myRead();
        if (ret != TRUE) {
            printf("ERROR!!!!\n");
            break;
        }
        myEval();
        myPrint();
    }
    return 0;
}
