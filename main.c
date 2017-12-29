#include <stdio.h>
#include <float.h>

#define MAXBUF 4096 /* buf size */
#define MAXTOKEN 4096 /* token size */
#define MAXMEMORY 4096
#define MAXSTRINGS 20

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
typedef enum {true, false} bool;

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
bool isNumber(struct token*);
bool isFloat (struct token*);
bool isString (struct token*);
bool copyString(struct token *, struct Data *);
int getData ();
bool isParlen (struct token* );
bool isParlenEnd(struct token*);
bool isParlenStart(struct token*);
bool isNil (struct token*);
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
bool readAtom (struct token* in, struct Data *data) {
    bool result;
    if (isNil(in) == true) {
        data->typeflag = NIL;
    } else if (isNumber(in) == true) {
        if (isFloat (in) == true) {
            data->typeflag = FLOAT;
            data->float_data = toFloat(in->tokenp,in->size);
        } else { /* Int */
            data->typeflag = INT;
            data->int_data = toInt(in);
        }
    } else if (isString(in) == true) {
        data->typeflag = STRING;
        result = copyString(in,data);
        if (result == false) {
            return false;
        }
    } else { /* Symbol */
        data->typeflag = SYMBOL;
        result = copyString (in, data);
        if (result == false) {
          return false;
        }
    }
    return true;
}

bool copyString(struct token *from, struct Data *to) {
    int i;

    if (from->size >=MAXSTRINGS) {
        return false;
    }

    for (i=0;i<from->size;i++) {
        to->char_data[i] = (char) from->tokenp[i];
    }

    return true;
}

bool readS (struct token *from, struct Data *to) {
    int i;
    bool result;
    struct Cons *nextCell;

    to->typeflag =CONS;
    if ((i = getConsCell()) == MAXBUF) {
        return false;
    }
    to->cons = &ConsCells[i];
    nextCell = &ConsCells[i];

    if (isParlenEnd (from->nextp) == true) {
      from = from->nextp;
      /* '() is as a atom */
      if ((i = getData()) == MAXBUF) {
        return false;
      }
      Datas[i].useflag = use;
      Datas[i].typeflag = NIL;
      nextCell->car = &Datas[i];
      nextCell->cdr = NULL;

      return true;
    }

    while (from->nextp != NULL) {
        from = from->nextp;
        if (isParlenEnd (from) == true) {
            /* end of S-exp */
            nextCell->cdr = NULL;
            return true;
        } else if ((isParlenStart (from) == true) && (isParlenEnd (from->nextp) == true)) {
            /* '() is as a atom */
            if ((i = getData ()) ==MAXBUF) {
                return false;
            }
            Datas[i].useflag = use;
            Datas[i].typeflag = NIL;
            nextCell->car = &Datas[i];
            if ((i = getConsCell ()) == MAXBUF) {
                return false;
            }
            nextCell->cdr = &ConsCells[i];
            nextCell = &ConsCells[i];
            nextCell->useflag = use;

        } else if (isParlenStart (from) == true) {
            /* start of S-exp */
            if ((i = getData ()) == MAXBUF) {
                return false;
            }
            to = &Datas[i];
            readS (from, to);
        } else {
            /* read Atom */
            if ((i = getData()) == MAXBUF) {
                return false;
            }
            to = &Datas[i];
            to->useflag = use;
            nextCell->car = &Datas[i];
            if (readAtom (from, to) == false) {
                return false;
            }
        }
    }
    return true;
}

bool myRead () {
    int i;
    i = getBuf (buf,MAXBUF);
    if (i > MAXBUF) {
        return false;
    }
    i = tokenize (buf, MAXBUF, tokens, MAXTOKEN);
    if (i > MAXTOKEN) {
        return false;
    }
    i = getData();
    if (i == MAXBUF) {
      return false;
    }
    Datas[i].useflag = use;
    /* S式ならreadS */
    /* S式でないならreadAtom */
    if (isParlen(&tokens[0]) == true) {
        if (readS(&tokens[0], &Datas[i]) == false) {
            return false;
        }
    } else {
        if (readAtom (&tokens[0],&Datas[i]) == false) {
            return false;
        }
    }

    return true;
}
bool isParlen (struct token* p) {
    bool result = false;
    if (p->size == 1) {
        if ( (p->tokenp[0] == '(') || (p->tokenp[0] == ')') ) {
            result = true;
        }
    }
    return result;
}
bool isParlenStart (struct token* p) {
    if (p->size == 1) {
        if (p->tokenp[0] == '(') {
            return true;
        }
    }
    return false;
}
bool isParlenEnd (struct token *p) {
  if (p->size == 1) {
    if (p->tokenp[0] == ')') {
      return true;
    }
  }
  return false;
}

bool isNil (struct token *p) {
    if (p->size == 3) {
        if (((p->tokenp[0] == 'n') || (p->tokenp[0] == 'N')) &&
           ((p->tokenp[1] == 'i') || (p->tokenp[1] == 'I')) &&
           ((p->tokenp[2] == 'l') || (p->tokenp[2] == 'L')) ) {
           return true;
        }
    }
    return false;
}
bool isNumber (struct token* x) {
    int i;
    for (i = 0;i < x->size;i++) {
        if (('0' <= x->tokenp[i]) && (x->tokenp[i] <= '9')) { /* number */
        } else if ((x->tokenp[i] == '+') || (x->tokenp[i] == '-')) { /* +- */
        } else if (x->tokenp[i] == '.') { /* . */
        } else {
            return false;
        }
    }
    return true;
}
bool isFloat (struct token* x) {
    int i;
    bool flag = false;
    for (i=0;i < x->size;i++) {
        if (x->tokenp[i] == '.') {
            flag = true;
            break;
        }
    }
    return flag;
}

bool isString (struct token* x) {
    bool result;
    if ( (x->tokenp[0] == '"') && (x->tokenp[x->size - 1] == '"')) {
        result = true;
    } else {
        result = false;
    }
    return result;
}
bool isSymbol (struct token* x) {
    if ((x->tokenp[0] == '(') || (x->tokenp[0] == ')')) {
        return false;
    }
    if (isNumber (x)) {
        return false;
    }
    return true;
}
bool isLiteral (struct token* x) {
    if ((x->tokenp[0] == '(') || (x->tokenp[0] == ')')) {
        return false;
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
bool isQuote (struct token* x) {
    bool result = false;
    if (x->size == 5) {
        if ((x->tokenp[0] == 'q') &&
            (x->tokenp[1] == 'u') &&
            (x->tokenp[2] == 'o') &&
            (x->tokenp[3] == 't') &&
            (x->tokenp[4] == 'e')) {
            result = true;
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
    if (isSymbol (x) == true) {
       m = findSymbol (x);
       return (toFloat (m->val, mySize(m->val)));
    /* 定数リテラル */
    } else if (isLiteral (x) == true) {
        return (toFloat (x->tokenp, x->size));
    /* (quote exp) */
    } else if (isQuote (x->nextp) == true) {
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
    bool ret;
    printf("my version lispy...\n");
    initCells();

    while (1) {
        printf ("Lispy >");
        ret = myRead();
        if (ret != true) {
            printf("ERROR!!!!\n");
            break;
        }
        myEval();
        myPrint();
    }
    return 0;
}
