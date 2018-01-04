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
enum typeflag { INT, FLOAT, STRING, CONS, SYMBOL, NIL, BOOL};
typedef enum {true, false} bool;

struct Data {
    enum typeflag typeflag;
    enum useflag useflag;
    int int_data;
    float float_data;
    char char_data[MAXSTRINGS];
    bool bool;
    struct Cons * cons;
};

struct Cons {
    struct Data * car; /* car */
    struct Cons * cdr; /* cdr */
    enum useflag useflag;
};


struct Cons ConsCells[MAXBUF];
struct Data Datas[MAXBUF];
struct Data *DefinePool;

int index_of_Consceslls;
int index_of_Datas;
int index_of_Read_Datas;

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
bool BI_Plus(struct Data*);
bool compString (char *, char *);
bool evalS (struct Data *);
bool evalAtom (struct Data *);
bool BI_define (struct Data *);
bool BI_lambda (struct Data *);
bool BI_if (struct Data *);
bool BI_loop (struct Data *);
bool BI_load (struct Data *);
void copyData (struct Data *, struct Data *);


struct functionName{
    char name[MAXSTRINGS];
    bool (*func)(struct Data*);
} BIfunc[] = {
    "+", BI_Plus,
    "",NULL /* terminator */
};
void putTokens() {
    int i,j;
    for (i=0;tokens[i].nextp!=NULL;i++) {
        for (j=0;j<tokens[i].size;j++) {
            putc(tokens[i].tokenp[j],stdout);
        }
        putc('\n',stdout);
    }
}
void putToken (struct token *p) {
    int i;
    for (i = 0;i<p->size;i++) {
        putc(p->tokenp[i],stdout);
    }
}
void putCells (struct Data* p) {
    struct Cons *n;
    if (p->typeflag == CONS) {
        printf("%d ",p->typeflag);
        n = p->cons;
        while(1){
            if ((n->cdr == NULL) && (n->car->typeflag == NIL)) {
                break;
            }
            /* typeflags */
            if (n->car->typeflag == CONS) {
                putCells(n->car);
            } else {
                printf("%d ",n->car->typeflag);
            }
            n = n->cdr;
        }
        printf ("\n");
    } else {
        /* one word */
        printf("%d\n",p->typeflag);
    }
}

int getBuf (int s[], int limit) {
    int c,i,ps,pe;
    for (i=0,ps=0,pe=0;(i<limit) && ((c=getchar())!=EOF) ;i++) {
        s[i] = c;
        if (c == '(') { /* ( */
            if ((pe < ps) || (ps == 0 && ps == pe)) {
                ps++;
            } else {
                i = limit;
                break;
            }
        } else if (c == ')') { /* ) */
            if ((pe < ps) || (pe == 0 && pe == ps)) {
                pe++;
            } else {
                i = limit;
                break;
            }
        } else if (c == '\n') {
            if (ps == pe) {
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
    for (i=0,o=0,t=0,m=OUT;(i<limit_in) && (o<limit_out) && (in[i]!='\0');i++,t++) {
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
    if (m == IN) {
        /* one word */
        out[o].size = t;
    }
    return o;
}
bool readAtom (struct token* in, struct Data *data) {
    bool result;
    /* putToken(in); dbg */
    if (isNil(in) == true) {
        data->typeflag = NIL;
        /* printf (" NIL, ");  dbg */
    } else if (isNumber(in) == true) {
        if (isFloat (in) == true) {
            data->typeflag = FLOAT;
            /* printf (" FLOAT, ");  dbg */
            data->float_data = toFloat(in->tokenp,in->size);
            if ((result = copyString(in,data)) == false) {
                return false;
            }
        } else { /* Int */
            data->typeflag = INT;
            /* printf (" INT, ");  dbg */
            data->int_data = toInt(in);
        }
    } else if (isString(in) == true) {
        data->typeflag = STRING;
        /* printf (" STRING, ");  dbg */
        result = copyString(in,data);
        if (result == false) {
            return false;
        }
    } else { /* Symbol */
        data->typeflag = SYMBOL;
        /* printf (" SYMBOL, ");  dbg */
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
struct token * getNextToken(struct token *from) {
    int ps = 1,pe = 0;
    while(ps != pe) {
        from = from->nextp;
        if (isParlenStart(from) == true) {
            ps++;
        } else if (isParlenEnd(from) == true) {
            pe++;
        }
    }
    return from;
}
bool readS (struct token *from, struct Data *to) {
    int i;
    bool result;
    struct Cons *nextCell;

    to->typeflag = CONS;
    if ((i = getConsCell()) == MAXBUF) {
        return false;
    }
    nextCell = &ConsCells[i];
    to->cons = nextCell;

    while (from->nextp != NULL) {
        from = from->nextp;
        if ((i= getData()) == MAXBUF) {
            return false;
        }
        to = &Datas[i];
        nextCell->car = to;

        if (isParlenEnd (from) == true) {
            /* end of S-exp */
            if ((i = getData ()) == MAXBUF) {
                return false;
            }
            to->typeflag = NIL;
            nextCell->cdr = NULL;
            return true;
        } else if ((isParlenStart (from) == true) && (isNil(from) == false)) {
            /* start of S-exp */
            /* printf ("[");  dbg */
            readS (from, to);
            from = getNextToken(from);
            /* printf ("] ");  dbg */
            if ((i = getConsCell ()) == MAXBUF) {
                return false;
            }
            nextCell->cdr = &ConsCells[i];
            nextCell = &ConsCells[i];
        } else {
            /* read Atom */
            if (readAtom (from, to) == false) {
                return false;
            }
            if ((isNil(from) == true) && (isParlenStart(from) == true) ) {
                /* from = '(', from->nextp = ')' */
                from = from->nextp;
            }
            if ((i = getConsCell ()) == MAXBUF) {
                return false;
            }
            nextCell->cdr = &ConsCells[i];
            nextCell = &ConsCells[i];
        }
    }
    return true;
}

bool myRead () {
    int i;
    i = getBuf (buf,MAXBUF);
    if (i >= MAXBUF) {
        return false;
    }
    i = tokenize (buf, MAXBUF, tokens, MAXTOKEN);
    if (i >= MAXTOKEN) {
        return false;
    }
    /* putTokens();  dbg */
    i = getData();
    if (i == MAXBUF) {
      return false;
    }
    /* S式ならreadS */
    /* S式でないならreadAtom */
    if ((isParlenStart(&tokens[0]) == true) &&
        (isParlenEnd(&tokens[1]) == false)) {  /* '() is as Atom */
        if (readS(&tokens[0], &Datas[i]) == false) {
            return false;
        }
    } else {
        if (readAtom (&tokens[0],&Datas[i]) == false) {
            return false;
        }
    }
    /* putCells(&Datas[i]);  dbg */
    index_of_Read_Datas = i;

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
    }else if ((p->size == 1)&&
              (isParlenStart(p) == true) &&
              (p->nextp->size == 1) &&
              (isParlenEnd(p->nextp) == true)) {
        return true;
    }
    return false;
}
bool isDigit (struct token* x, int j) {
    int i,c,size= x->size;
    bool result = false;
    for (i = j;i < size;i++) {
        c = x->tokenp[i];
        if (('0' <= c) && (c <= '9')) { /* number */
        } else if (c == '.') { /* . */
        } else {
            break;
        }
    }
    if (size <= i) {
        result = true;
    }
    return result;
}
bool isNumber (struct token* x) {
    int i;
    bool result = false;

    if (x->size == 2) {
        if (((x->tokenp[0] == '+') || (x->tokenp[0] == '-')) &&
            (('0' <= x->tokenp[1]) && (x->tokenp[1] <= '9'))) {
            /* start from +/- */
            result = true;
        } else {
            /* start from 0-9 */
            result = isDigit(x, 0);
        }
    } else if (2 < x->size) {
        if ((x->tokenp[0] == '+') || (x->tokenp[0] == '-') ) {
            if (('0' <= x->tokenp[1]) && (x->tokenp[1] <= '9')) {
                result = isDigit(x, 2);
            }
        } else {
            /* start from 0-9 */
            result = isDigit(x, 0);
        }
    } else {
        /* start from 0-9 */
        result = isDigit(x, 0);
    }
    return result;
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
    for(i=0,result=0;i<p->size;i++) {
        result = result + p->tokenp[i] - '0';
        if (i < p->size - 1) {
            result = result * 10;
        }
    }
    return result;
}

float toFloat(int *p, int size) {
    float ret,dec;
    int i,c,n = 0;
    bool pm = false; /* plus-minus */

    for (i = 0,ret = 0;i < size;i++) {
        c = p[i];
        if (c == '-') { /* +- */
            pm = true;
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
        ret = ret * myPow (n - 1);
    }
    if (pm == true) {
        ret = ret * -1;
    }
    return ret;
}

int mySize(int *x) {
    int i;
    for (i = 0;x[i] != '\0';i++) {;}
    return i-1;
}
void initCells() {
    int i;
    for (i=0;i<MAXBUF;i++) {
      ConsCells[i].useflag = not_use;
      Datas[i].useflag = not_use;
    }
    index_of_Consceslls = 0;
    index_of_Datas = 0;
    DefinePool = NULL;
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
    if (i != MAXBUF) {
        ConsCells[i].useflag = use;
    }
    return i;
}
void freeConsCell (struct Cons *c) {
    c->useflag = not_use;
}
void freeData (struct Data *d) {
    d->useflag = not_use;
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
    if (i != MAXBUF) {
        Datas[i].useflag = use;
    }
    return i;
}

bool compString (char *a, char *b) {
    int size,i;
    bool ret = false;
    if ((size = sizeof(a)) == sizeof(b)) {
        if (size == 0) {
            return true;
        }
        for(i=0;(i<size) && ((a[i] != '\0') || (b[i] != '\0'));i++) {
            if (a[i] != b[i]) {
                break;
            }
        }
    }
    if ((i == size) ||
        ((a[i] == b[i]) && a[i] == '\0')) {
        ret = true;
    }
    return ret;
}
bool (*findFunction (struct Data *d))(struct Data *) {
    int i = 0;
    bool ret;
    char * func = d->char_data;
    while(BIfunc[i].func != NULL) {
        if ((ret = compString(BIfunc[i].name, func)) == true) {
            return BIfunc[i].func;
        }
        i++;
    }
    return NULL;
}
bool spForm (struct Data *d) {
    bool ret = true;
    char *car = d->cons->car->char_data;

    if (((ret = compString (car, "define")) == true) ||
        ((ret = compString (car, "DEFINE")) == true)) {
        /* define */
        ret = BI_define (d);
    } else if (((ret = compString (car, "lambda")) == true) ||
               ((ret = compString (car, "LAMBDA")) == true)) {
        /* lambda */
        ret = BI_lambda (d);
    } else if (((ret = compString (car, "quote")) == true) ||
               ((ret = compString (car, "QUOTE")) == true)) {
        /* quote */
    } else if (((ret = compString (car, "if")) == true) ||
               ((ret = compString (car, "IF")) == true)) {
        /* if */
        ret = BI_if (d);
    } else if (((ret = compString (car, "loop")) == true) ||
               ((ret = compString (car, "LOOP")) == true)) {
        /* loop */
        ret = BI_loop (d);
    } else if (((ret = compString (car, "load")) == true) ||
               ((ret = compString (car, "LOAD")) == true)) {
        /* load */
        ret = BI_load (d);
    }
    return ret;
}
bool apply (struct Data *d) {
    bool (*func)(struct Data *);
    bool ret = true;
    if (d->cons->car->typeflag != SYMBOL) {
        printf ("Can't apply this CAR.\n");
        ret = false;
    } else if ((func = findFunction(d->cons->car)) != NULL) {
        ret = func (d);
    } else {
        /* Special Form */
        ret = spForm(d);
    }
    return ret;
}

bool evalEach (struct Data *d) {
    bool ret = true;;
    struct Cons *cons = d->cons;
    while (cons->cdr!=NULL) {
        if (cons->car->typeflag == CONS) {
            ret = evalS (cons->car);
            if (ret == false) {
                break;
            }
        } else {
            ret = evalAtom (cons->car);
            if (ret == false) {
                break;
            }
        }
        cons = cons->cdr;
    }
    return ret;
}
struct Data * findSymbol(struct Data *d) {
    struct Data *ret = NULL;
    struct Cons *pool;
    if ((d->typeflag != SYMBOL) || (DefinePool == NULL)) {
        ret = NULL;
    } else {
        pool = DefinePool->cons;
        while (pool != NULL) {
            if (compString (d->char_data, pool->car->char_data) == true) {
                ret = pool->cdr->car;
            }
            if (pool->cdr == NULL) {
                break;
            }
            pool = pool->cdr->cdr;
        }
    }
    return ret;
}
void copyData (struct Data *from, struct Data *to) {
    int i;
    to->typeflag = from->typeflag;
    to->int_data = from->int_data;
    to->float_data = from->float_data;
    to->cons = from->cons;
    for (i=0;i<MAXSTRINGS;i++) {
        to[i] = from[i];
    }
}
bool evalSymbol (struct Data *d) {
    bool (*func)(struct Data *);
    int i;
    struct Data *d_ret;
    bool ret = true;
    if ((d_ret = findSymbol(d)) != NULL) {
        /* defined symbol */
        copyData (d_ret, d);
    } else if ((func = findFunction (d)) != NULL) {
        /* built-in function */
    } else {
        /* special form */
    }
    return ret;
}
bool evalS (struct Data *d) {
    bool ret = true;
    ret = evalEach (d);
    if (ret == false) {
        return ret;
    }
    ret = apply (d);
    if (ret == false) {
        return ret;
    }
    return ret;
}
bool evalAtom (struct Data *d) {
    bool ret = true;
    enum typeflag f = d->typeflag;
    if (f == CONS) {
        ret = false;
    } else if (f == SYMBOL) {
        ret = evalSymbol (d);
    } else {
        /* INT, FLOAT, STRING, NIL */
    }
    return ret;
}
bool myEval () {
    /* index_of_Read_Datas */
    bool ret = true;
    struct Data *d = &Datas[index_of_Read_Datas];

    if (d->typeflag == CONS) {
        ret = evalS(d);
        if (ret == false) {
            return false;
        }
    } else {
        ret = evalAtom(d);
        if (ret == false) {
            return false;
        }
    }
    return ret;
}

bool printS (struct Data *d) {
    return true;
}
bool printAtom (struct Data *d) {
    if (d->typeflag == INT) {
        printf ("%d\n",d->int_data);
    } else if (d->typeflag == FLOAT) {
        printf ("%f\n",d->float_data);
    } else if (d->typeflag == STRING) {
        printf ("%s\n",d->char_data);
    } else if (d->typeflag == SYMBOL) {
        printf ("%s\n",d->char_data);
    } else if (d->typeflag == NIL) {
        printf ("NIL\n");
    }
    freeData (d);
    return true;
}
bool myPrint () {
    bool ret = true;
    struct Data *d = &Datas[index_of_Read_Datas];
    if (d->typeflag == CONS) {
        ret = printS (d);
        if (ret == false) {
            return false;
        }
    } else {
        ret = printAtom (d);
        if (ret == false) {
            return false;
        }
    }
    return ret;
}
int main () {
    bool ret;
    printf("my version lispy...\n");
    initCells();

    while (1) {
        printf ("Lispy > ");
        ret = myRead();
        if (ret != true) {
            printf("READ ERROR!!!!\n");
            break;
        }
        ret = myEval();
        if (ret != true) {
            printf("EVAL ERROR!!!!\n");
            break;
        }
        ret = myPrint();
        if (ret == false) {
            printf("PRINT ERROR!!!!\n");
            break;
        }
    }
    return 0;
}


/* Build-in Functions */
bool BI_Plus (struct Data *d) {
    bool ret = true,isfloat = false;
    int ret_int;
    float ret_float;
    struct Cons *args = d->cons->cdr, *c;

    /* all INT or not*/
    while (args->cdr != NULL) {
        if (args->car->typeflag == FLOAT) {
            isfloat = true;
            break;
        }
        args = args->cdr;
    }

    args = d->cons->cdr;
    if (isfloat == true) {
        ret_float = 0;
        while (args->cdr != NULL) {
            if (args->car->typeflag ==FLOAT) {
                ret_float += args->car->float_data;
            } else {
                ret_float += (float) args->car->int_data;
            }
            freeData(args->car);
            c = args;
            args = args->cdr;
            freeConsCell (c);
        }
        if ((args->cdr != NULL) && (args->cdr->useflag == use)) {
            freeData(args->car);
            freeConsCell(args);
        }
        freeConsCell(d->cons);
        d->typeflag = FLOAT;
        d->float_data = ret_float;
    } else {
        ret_int = 0;
        while (args->cdr != NULL) {
            ret_int += args->car->int_data;
            freeData (args->car);
            c = args;
            args = args->cdr;
            freeConsCell (c);
        }
        if ((args->cdr != NULL) && (args->cdr->useflag == use)) {
            freeData(args->car);
            freeConsCell(args);
        }
        freeConsCell(d->cons);
        d->typeflag = INT;
        d->int_data = ret_int;
    }
    return ret;
}
bool BI_define (struct Data *d) {
    int i;
    bool ret = true;
    /* DefinePool */
    if (DefinePool == NULL) {
        if ((i = getData ()) == MAXBUF) {
            ret = false;
        }
        DefinePool = & Datas[i];
        DefinePool->typeflag = CONS;
    } else {
        d->cons->cdr->cdr->cdr = DefinePool->cons;
    }
    DefinePool->cons = d->cons->cdr;
    freeData (d->cons->car);
    freeConsCell (d->cons);
    d->typeflag = DefinePool->cons->car->typeflag;
    for (i=0;i<MAXSTRINGS;i++) {
        d->char_data[i] = DefinePool->cons->car->char_data[i];
    }
    return ret;
}
bool BI_lambda (struct Data *d) {
    return true;
}
bool BI_if (struct Data *d) {
    return true;
}
bool BI_loop (struct Data *d) {
    return true;
}
bool BI_load (struct Data *d) {
    return true;
}
