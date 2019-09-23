#include <stdio.h>
#include <float.h>

#define MAXBUF 4096 /* buf size */
#define MAXTOKEN 4096 /* token size */
#define MAXMEMORY 4096
#define MAXSTRINGS 20

static int buf[MAXBUF];
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
    bool bool;
    struct Cons * cons;
    char char_data[MAXSTRINGS];
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
bool copySTRINGS(struct token *, struct Data *);
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
bool eval_if (struct Data *);
bool eval_cond (struct Data *);
bool eval_progn (struct Data *);
bool eval_let (struct Data *);
int eval_let_helper_set_DefinePools(struct Data *);
void eval_let_helper_free_DefinePools(int );
bool evalAtom (struct Data *);
bool evalEach (struct Data *d);
bool BI_define (struct Data *);
bool BI_lambda (struct Data *);
bool BI_loop (struct Data *);
bool BI_load (struct Data *);
bool BI_equal (struct Data *);
bool BI_less_than (struct Data *);
bool BI_greater_than (struct Data *);
bool BI_less_equal (struct Data *);
bool BI_greater_equal (struct Data *);
bool BI_quote (struct Data *);
bool BI_list (struct Data *);
bool BI_car (struct Data *);
bool BI_cdr (struct Data *);
bool BI_atom (struct Data *);
void copyData (struct Data *, struct Data *);
bool eval_co_each (struct Cons *);
bool initDefinePood();
void freeConsCells (struct Cons *);
void freeConsCell (struct Cons *);
void freeData (struct Data *);
bool equalS (struct Data *a, struct Data *b);
void mystrcpy (char *from, char *to);
bool printAtom (struct Data *d);
bool printAtom_withoutFree (struct Data *);
bool printS_withoutFree (struct Data *);
bool BI_cons (struct Data *d);
int setDefinePoolS (struct Data *, struct Data *);
void freeDefinePoolS (int);
bool copyConsData (struct Data *, struct Data *);
bool BI_minus (struct Data *);
bool BI_minus_float (struct Data *);
bool BI_minus_int (struct Data *);
bool BI_mult (struct Data *);
bool BI_mult_int (struct Data *);
bool BI_mult_float (struct Data *);
bool BI_set (struct Data *);
bool is_atom (struct Data *);
bool is_list (struct Data *);

struct functionName{
    char name[MAXSTRINGS];
    bool (*func)(struct Data*);
} BIfunc[] = {
    "+", BI_Plus,
    "=", BI_equal,
    "<", BI_less_than,
    ">", BI_greater_than,
    "<=", BI_less_equal,
    ">=", BI_greater_equal,
    "car", BI_car,
    "cdr", BI_cdr,
    "cons", BI_cons,
    "-", BI_minus,
    "*", BI_mult,
    "atom?", BI_atom,
    "list", BI_list,
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
    bool f;
    if ((f = is_list (p)) == true) {
        printf("%d ",p->typeflag);
        n = p->cons;
        while(1){
            if ((n->cdr == NULL) && (n->car->typeflag == NIL)) {
                break;
            }
            /* typeflags */
            if ((f = is_list (n->car)) == true) {
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
    int c,i,paren = 0;
    for (i=0;(i<limit) && ((c=getchar())!=EOF) ;i++) {
        if ((c == '\n') || (c == '\t')) {
            s[i]  = ' ';
	} else {
            s[i] = c;
	}
        if (c == '(') { /* ( */
            paren++;
        } else if (c == ')') { /* ) */
            paren--;
        } else if (c == '\n') {
		if (paren == 0) {
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
        result = copySTRINGS(in,data);
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
bool copySTRINGS(struct token *from, struct Data *to) {
    int i,j;

    if (from->size >=MAXSTRINGS) {
        return false;
    }

    for (j=0,i=1;i<from->size;j++,i++) {
        to->char_data[j] = (char) from->tokenp[i];
    }

    to->char_data[j - 1] = 0x00;
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
void freeConsCells (struct Cons *c) {
    struct Cons *cons = c, *n = c->cdr, *tmp;
    bool f;
    while (n != NULL) {
        if (cons->car == NULL) {
        } else {
            if ((f = is_list (cons->car)) == true) {
                freeConsCells (cons->car->cons);
            }
            freeData (cons->car);
        }
        freeConsCell (cons);
        tmp = n->cdr;
        cons = n;
        n = tmp;
    }
}
void freeConsCell (struct Cons *c) {
    c->useflag = not_use;
    c->car = NULL;
    c->cdr = NULL;
}
void freeData (struct Data *d) {
    int i;
    d->useflag = not_use;
    d->cons = NULL;
    for (i=0;i<MAXSTRINGS;i++) {
        d->char_data[i] = 0;
    }
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

int mysizeof(char *a, int maxsize) {
    int ret;
    char *p = a;
    while (*p != '\0') {p++;}

    ret = p - a;
    if (maxsize < ret) {
        ret = maxsize;
    }
    return ret;
}
int mystrcmp (char *s, char *t) {
    for ( ; *s == *t; s++,t++) {
        if (*s == '\0') {
            return 0;
        }
   }
   return *s - *t;
}
bool compString (char *a, char *b) {
    int size,i;
    bool ret = false;
    if ((size = mysizeof(a,MAXSTRINGS)) == mysizeof(b,MAXSTRINGS)) {
        if (size == 0) {
            return true;
        } else if ((i = mystrcmp (a, b)) == 0) {
            return true; 
        }
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
    bool ret = true, f;
    char *car = d->cons->car->char_data;

    if (((ret = compString (car, "define")) == true) ||
        ((ret = compString (car, "DEFINE")) == true)) {
        /* define */
        ret = BI_define (d);
    } else if ((f = is_list (d->cons->car)) == true) {
        if (((ret = compString (d->cons->car->cons->car->char_data, "lambda")) == true) ||
               ((ret = compString (d->cons->car->cons->car->char_data, "LAMBDA")) == true)) {
            /* lambda */
            ret = BI_lambda (d);
        }
    } else if (((ret = compString (car, "lambda")) == true) ||
               ((ret = compString (car, "LAMBDA")) == true)) {
    } else if (((ret = compString (car, "list")) == true) ||
               ((ret = compString (car, "LIST")) == true)) {
        ret = BI_list (d);
    } else if (((ret = compString (car, "quote")) == true) ||
               ((ret = compString (car, "QUOTE")) == true)) {
        /* quote */
        ret = BI_quote (d);
        /* if */
    } else if (((ret = compString (car, "loop")) == true) ||
               ((ret = compString (car, "LOOP")) == true)) {
        /* loop */
        ret = BI_loop (d);
    } else if (((ret = compString (car, "load")) == true) ||
               ((ret = compString (car, "LOAD")) == true)) {
        /* load */
        ret = BI_load (d);
    } else if (((ret = compString (car, "set")) == true) ||
               ((ret = compString (car, "SET")) == true)) {
        /* set */
        ret = BI_set (d);
    }
    return ret;
}
bool apply (struct Data *d) {
    bool (*func)(struct Data *);
    bool ret = true, f;
    struct Data *tmpData;
    if ((f = is_atom (d)) == true) {
    } else if (d->cons->car->typeflag != SYMBOL) {
        if (((f = is_list (d->cons->car)) == true) && 
            ((compString (d->cons->car->cons->car->char_data, "lambda") == true) ||
             (compString (d->cons->car->cons->car->char_data, "LAMBDA") == true))) {
            spForm (d);
        } else {
            printf ("Can't apply this CAR.\n");
            ret = false;
        }
    } else if ((func = findFunction(d->cons->car)) != NULL) {
        ret = func (d);
    } else {
        /* Special Form */
        ret = spForm(d);
    }
    return ret;
}

bool eval_co_each (struct Cons *cons) {
    int i;
    bool ret = true, f;
    struct Cons *n = cons, *tmp;
    while ((n != NULL) && (n->cdr!=NULL)) {
        if ((f = is_list (n->car)) == true) {
            ret = evalS (n->car);
            if (ret == false) {
                break;
            }
        } else {
            ret = evalAtom (n->car);
            if (ret == false) {
                break;
            }
        }
        n = n->cdr;
    }
    return ret;
}
bool eval_if (struct Data *d) {
    bool ret = true, f;
    struct Cons *tmpCons;
    struct Data *cond_p, *then_p, *else_p, tmpData;

    cond_p = d->cons->cdr->car;
    then_p = d->cons->cdr->cdr->car;
    else_p = d->cons->cdr->cdr->cdr->car;

    ret = evalS(cond_p);
    if (ret == false) {
        return false;
    }
    if (cond_p->typeflag == BOOL) {
        if (cond_p->bool == true) {
            if ((f = is_list (then_p)) == true) {
                tmpCons = then_p->cons;
                then_p->cons = NULL;
                then_p->typeflag = INT;
            } else {
                tmpCons = NULL;
                copyData (then_p, &tmpData);
                tmpData.useflag = use;
            }
        } else {
            if ((f = is_list (else_p)) == true) {
                tmpCons = else_p->cons;
                else_p->cons = NULL;
                else_p->typeflag = INT;
            } else {
                tmpCons = NULL;
                copyData (else_p, &tmpData);
                tmpData.useflag = use;
            }
        }
        if (tmpCons != NULL) {
            freeConsCells (d->cons);
            d->cons = tmpCons;
            evalEach(d);
        } else {
            freeConsCells (d->cons);
            copyData (&tmpData, d);
            evalAtom(d);
        }
    }else {
        ret = false;
    }
    return ret;
}
bool eval_cond (struct Data *d) {
    bool ret = false, f;
    struct Data *case_p, *cond_p, *then_p, tmpData;
    struct Cons *tmp_p, *tmpCons;

    tmp_p = d->cons->cdr;
    while ((tmp_p->car->typeflag != NIL) && (tmp_p->cdr != NULL)) {
        /* form check */
        if ((f = is_list (tmp_p->car)) == false) {
            break;
        }
        /* condition check */
        cond_p = tmp_p->car->cons->car;
        then_p = tmp_p->car->cons->cdr->car;

        /* for ELSE */
        if ((cond_p->typeflag == SYMBOL) &&
            ((compString (cond_p->char_data, "else") == true) ||
             (compString (cond_p->char_data, "ELSE") == true))) {
            ret = true;
            cond_p->typeflag = BOOL;
            cond_p->bool = true;
        } else {
            ret = evalS(cond_p);
        }
        if (ret == false) {
            break;
        }
        if (cond_p->typeflag != BOOL) {
            break;
        }
        if (cond_p->bool == true) {
            if ((f = is_list (then_p)) == true) {
                tmpCons = then_p->cons;
                then_p->cons = NULL;
                then_p->typeflag = INT;
                freeConsCells (d->cons);
                d->cons = tmpCons;
                /* eval then_p */
                evalEach(d);
                ret = true;
                break;
            } else {
                copyData (then_p, &tmpData);
                tmpData.useflag = use;
                freeConsCells (d->cons);
                copyData (&tmpData, d);
                /* eval then_p */
                evalAtom (d);
                ret = true;
                break;
            }
        }

        tmp_p = tmp_p->cdr;
    }

    return ret;
}
bool evalEach (struct Data *d) {
    bool ret = true, f;
    char * s = d->cons->car->char_data;
    struct Cons *tmpCons;
    struct Data tmpData;

    if ((compString (s, "define") == true) ||
        (compString (s, "DEFINE") == true)) {
        ret = eval_co_each(d->cons->cdr->cdr);
    } else if (((f = is_list (d->cons->car)) == true) &&
                 ((compString (d->cons->car->cons->car->char_data, "lambda") == true) ||
                  (compString (d->cons->car->cons->car->char_data, "LAMBDA") == true))) {
    } else if ((compString (s, "lambda") == true) ||
               (compString (s, "LAMBDA") == true)) {
    } else if ((compString (s, "list") == true) ||
               (compString (s, "LIST") == true)) {
        ret = eval_co_each(d->cons->cdr);
    } else if ((compString (s, "quote") == true) ||
               (compString (s, "QUOTE") == true)) {
    } else if ((compString (s, "if") == true) ||
               (compString (s, "IF") == true)) {
        ret = eval_if (d);
    } else if ((compString (s, "cond") == true) ||
               (compString (s, "COND") == true)) {
        ret = eval_cond (d);
    } else if ((compString (s, "loop") == true) ||
               (compString (s, "LOOP") == true)) {
        ret = eval_co_each(d->cons);
    } else if ((compString (s, "load") == true) ||
               (compString (s, "LOAD") == true)) {
    } else if ((compString (s, "set") == true) ||
               (compString (s, "SET") == true)) {
        ret = eval_co_each(d->cons->cdr->cdr);
        if (ret == false) {
            return ret;
        }
    } else if ((compString (s, "progn") == true) ||
               (compString (s, "PROGN") == true)) {
        ret = eval_progn (d);
    } else if ((compString (s, "let") == true) ||
               (compString (s, "let") == true)) {
        ret = eval_let (d);
    } else {
        ret = eval_co_each(d->cons);
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
            if ((pool->car->typeflag == SYMBOL) && (compString (d->char_data, pool->car->char_data) == true)) {
                ret = pool->cdr->car;
                break;
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
    to->bool = from->bool;
    for (i=0;i<MAXSTRINGS;i++) {
        to->char_data[i] = from->char_data[i];
    }
}
bool copyConsData (struct Data *from, struct Data *to) {
    int i;
    bool ret = true, f;
    struct Cons *nf = from->cons, *nt;
    copyData (from, to);
    if ((i = getConsCell ()) == MAXBUF) {
        return false;
    }
    nt = &ConsCells[i];
    to->cons = nt;
    while ((nf->car->typeflag != NIL) && (nf->cdr != NULL)) {
        if ((i = getData ()) == MAXBUF) {
            ret = false;
            break;
        }
        nt->car = &Datas[i];
        copyData (nf->car, nt->car);
        if ((f = is_list (nf->car)) == true) {
            if ((ret = copyConsData (nf->car, nt->car)) == false) {
                break;
            }
        }
        if ((i = getConsCell ()) == MAXBUF) {
            ret = false;
            break;
        }
        nt->cdr = &ConsCells[i];
        nf = nf->cdr;
        nt = nt->cdr;
    }
    if ((i = getData ()) == MAXBUF) {
        ret = false;
    }
    nt->car = & Datas[i];
    nt->car->typeflag = NIL;
    nt->cdr = NULL;

    return ret;
}
bool evalSymbol (struct Data *d) {
    bool (*func)(struct Data *), f;
    int i;
    struct Data *d_ret;
    bool ret = true;
    if ((d_ret = findSymbol(d)) != NULL) {
        /* defined symbol */
        if ((f = is_list (d_ret)) == true) {
            ret = copyConsData (d_ret, d);
        } else {
            copyData (d_ret, d);
        }
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
    bool ret = true, f;
    struct Data *d = &Datas[index_of_Read_Datas];

    if ((f = is_list (d)) == true) {
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

bool printS_withoutFree (struct Data *d) {
    struct Cons *n = d->cons;
    bool f;

    printf ("(");
    while (n->cdr != NULL) {
        if (n != d->cons) {
            printf (" ");
        }
        if ((f = is_list (n->car)) == true) {
            printS_withoutFree(n->car);
        }
        printAtom_withoutFree (n->car);
        n = n->cdr;
    }
    printf (")");
    return true;
}
bool printS (struct Data *d) {
    struct Cons *n = d->cons;
    bool f;

    printf ("(");
    while (n->cdr != NULL) {
        if (n != d->cons) {
            printf (" ");
        }
        if ((f = is_list (n->car)) == true) {
            printS_withoutFree(n->car);
        }
        printAtom_withoutFree (n->car);
        n = n->cdr;
    }
    printf (")");
    freeConsCells (d->cons);
    freeData (d);
    return true;
}
bool printAtom_withoutFree (struct Data *d) {
    if (d->typeflag == INT) {
        printf ("%d",d->int_data);
    } else if (d->typeflag == FLOAT) {
        printf ("%f",d->float_data);
    } else if (d->typeflag == STRING) {
        printf ("%s",d->char_data);
    } else if (d->typeflag == SYMBOL) {
        printf ("%s",d->char_data);
    } else if (d->typeflag == NIL) {
        printf ("NIL");
    } else if (d->typeflag == BOOL) {
        if (d->bool == true) {
            printf ("TRUE");
        } else {
            printf ("FALSE");
        }
    }
    return true;
}
bool printAtom (struct Data *d) {
    printAtom_withoutFree (d);
    freeData (d);
    return true;
}
bool myPrint () {
    bool ret = true, f;
    struct Data *d = &Datas[index_of_Read_Datas];
    if ((f = is_list (d)) == true) {
        ret = printS (d);
        printf("\n");
        if (ret == false) {
            return false;
        }
    } else {
        ret = printAtom (d);
        printf("\n");
        if (ret == false) {
            return false;
        }
    }
    return ret;
}
int main () {
    bool ret;
    printf("my version lisp...\n");
    initCells();
    initDefinePood();

    while (1) {
        printf ("Lisp > ");
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
bool equalS (struct Data *a, struct Data *b) {
    bool ret = false;
    struct Data *s, *p;
    struct Cons *t = a->cons, *q = b->cons;
    enum typeflag flag;

    while ((t->cdr != NULL) && (q->cdr != NULL)) {
        s = t->car, p = q->car;
        if (s->typeflag == p->typeflag) {
            flag = s->typeflag;
            if (flag == INT) {
                if (s->int_data != p->int_data) {
                    ret = false;
                    break;
                }
            } else if (flag == FLOAT) {
                if (s->float_data != p->float_data) {
                    ret = false;
                    break;
                }
            } else if (flag == STRING) {
                if (compString(s->char_data, p->char_data) == false) {
                    ret = false;
                    break;
                }
            } else if (flag == CONS) {
                if (equalS (s, p) == false) {
                }
            } else if (flag == SYMBOL) {
                if (compString(s->char_data, p->char_data) == false) {
                    ret = false;
                    break;
                }
            } else if (flag == NIL) {
                if ((t->cdr == NULL) || (q->cdr == NULL)) {
                    ret = false;
                    break;
                }
            } else if (flag == BOOL) {
                if (s->bool != p->bool) {
                    ret = false;
                    break;
                }
            }
        } else {
            ret = false;
            break;
        }
        t = t->cdr;
        q = q->cdr;
    }

    if ((t->cdr == NULL) && (q->cdr == NULL)) {
        ret = true;
    }
    return ret;
}

bool BI_equal (struct Data *d) {
/*      =      <a1>    <a2> .... NIL
  *d -> [][] -> [][] -> [][]...-> [][]
*/
    bool ret = true, f;
    struct Cons *n = d->cons->cdr, *nn = d->cons->cdr->cdr;
    struct Data *tmp;

    while(nn->cdr != NULL) {
        if (n->car->typeflag == nn->car->typeflag) {
            if (n->car->typeflag == INT) {
                if (n->car->int_data != nn->car->int_data) {
                    ret = false;
                    break;
                }
            } else if (n->car->typeflag == FLOAT) {
                if (n->car->float_data != nn->car->float_data) {
                    ret = false;
                    break;
                }
            } else if (n->car->typeflag == STRING) {
                if (compString (n->car->char_data, nn->car->char_data) == false) {
                    ret = false;
                    break;
                }
            } else if (n->car->typeflag == SYMBOL) {
                if (compString (n->car->char_data, nn->car->char_data) == false) {
                    ret = false;
                    break;
                }
            } else if ((f = is_list (n->car)) == true) {
                if (equalS (n->car, nn->car) == false) {
                    ret = false;
                    break;
                }
            } else if (n->car->typeflag == NIL) {
                if (nn->car->typeflag != NIL) {
                    ret = false;
                    break;
                }
            } else if (n->car->typeflag == BOOL) {
                if (n->car->bool != nn->car->bool) {
                    ret = false;
                    break;
                }
            } else {
                ret = false;
                break;
            }
        } else {
            ret = false;
            break;
        }

        n = nn;
        nn = nn->cdr;
    }

    freeConsCells (d->cons);
    d->typeflag = BOOL;
    d->cons = NULL;
    if (ret == true) {
        d->bool = true;
    } else {
        d->bool = false;
    }
    return true;
}
bool BI_less_than(struct Data *d) {
/*      <      <a1>    <a2> .... NIL
  *d -> [][] -> [][] -> [][]...-> [][]
*/
    bool ret = true;
    struct Cons *n = d->cons->cdr, *nn = d->cons->cdr->cdr;
    struct Data *tmp;

    while((nn->car->typeflag != NIL) && (nn->cdr != NULL)) {
        if (n->car->typeflag == INT) {
            if (nn->car->typeflag == INT) {
                if (n->car->int_data >= nn->car->int_data) {
                    ret = false;
                    break;
                }
            } else if (nn->car->typeflag == FLOAT) {
                if (n->car->int_data >= nn->car->float_data) {
                    ret = false;
                    break;
                }
            } else {
                ret = false;
                break;
            }
        } else if (n->car->typeflag == FLOAT) {
            if (nn->car->typeflag == INT) {
                if (n->car->float_data >= nn->car->int_data) {
                    ret = false;
                    break;
                }
            } else if (nn->car->typeflag == FLOAT) {
                if (n->car->float_data >= nn->car->float_data) {
                    ret = false;
                    break;
                }
            } else {
                ret = false;
                break;
            }
        } else {
            ret = false;
            break;
        }
        n = nn;
        nn = nn->cdr;
    }

    freeConsCells (d->cons);
    d->typeflag = BOOL;
    d->cons = NULL;
    if (ret == true) {
        d->bool = true;
    } else {
        d->bool = false;
    }
    return true;
}
bool BI_greater_than(struct Data *d) {
/*      >      <a1>    <a2> .... NIL
  *d -> [][] -> [][] -> [][]...-> [][]
*/
    bool ret = true;
    struct Cons *n = d->cons->cdr, *nn = d->cons->cdr->cdr;
    struct Data *tmp;

    while((nn->car->typeflag != NIL) && (nn->cdr != NULL)) {
        if (n->car->typeflag == INT) {
            if (nn->car->typeflag == INT) {
                if (n->car->int_data <= nn->car->int_data) {
                    ret = false;
                    break;
                }
            } else if (nn->car->typeflag == FLOAT) {
                if (n->car->int_data <= nn->car->float_data) {
                    ret = false;
                    break;
                }
            } else {
                ret = false;
                break;
            }
        } else if (n->car->typeflag == FLOAT) {
            if (nn->car->typeflag == INT) {
                if (n->car->float_data <= nn->car->int_data) {
                    ret = false;
                    break;
                }
            } else if (nn->car->typeflag == FLOAT) {
                if (n->car->float_data <= nn->car->float_data) {
                    ret = false;
                    break;
                }
            } else {
                ret = false;
                break;
            }
        } else {
            ret = false;
            break;
        }
        n = nn;
        nn = nn->cdr;
    }

    freeConsCells (d->cons);
    d->typeflag = BOOL;
    d->cons = NULL;
    if (ret == true) {
        d->bool = true;
    } else {
        d->bool = false;
    }
    return true;
}
bool BI_less_equal(struct Data *d) {
/*      <=     <a1>    <a2> .... NIL
  *d -> [][] -> [][] -> [][]...-> [][]
*/
    bool ret = true;
    struct Cons *n = d->cons->cdr, *nn = d->cons->cdr->cdr;
    struct Data *tmp;

    while((nn->car->typeflag != NIL) && (nn->cdr != NULL)) {
        if (n->car->typeflag == INT) {
            if (nn->car->typeflag == INT) {
                if (n->car->int_data > nn->car->int_data) {
                    ret = false;
                    break;
                }
            } else if (nn->car->typeflag == FLOAT) {
                if (n->car->int_data > nn->car->float_data) {
                    ret = false;
                    break;
                }
            } else {
                ret = false;
                break;
            }
        } else if (n->car->typeflag == FLOAT) {
            if (nn->car->typeflag == INT) {
                if (n->car->float_data > nn->car->int_data) {
                    ret = false;
                    break;
                }
            } else if (nn->car->typeflag == FLOAT) {
                if (n->car->float_data > nn->car->float_data) {
                    ret = false;
                    break;
                }
            } else {
                ret = false;
                break;
            }
        } else {
            ret = false;
            break;
        }
        n = nn;
        nn = nn->cdr;
    }

    freeConsCells (d->cons);
    d->typeflag = BOOL;
    d->cons = NULL;
    if (ret == true) {
        d->bool = true;
    } else {
        d->bool = false;
    }
    return true;
}
bool BI_greater_equal(struct Data *d) {
/*      >=     <a1>    <a2> .... NIL
  *d -> [][] -> [][] -> [][]...-> [][]
*/
    bool ret = true;
    struct Cons *n = d->cons->cdr, *nn = d->cons->cdr->cdr;
    struct Data *tmp;

    while((nn->car->typeflag != NIL) && (nn->cdr != NULL)) {
        if (n->car->typeflag == INT) {
            if (nn->car->typeflag == INT) {
                if (n->car->int_data < nn->car->int_data) {
                    ret = false;
                    break;
                }
            } else if (nn->car->typeflag == FLOAT) {
                if (n->car->int_data < nn->car->float_data) {
                    ret = false;
                    break;
                }
            } else {
                ret = false;
                break;
            }
        } else if (n->car->typeflag == FLOAT) {
            if (nn->car->typeflag == INT) {
                if (n->car->float_data < nn->car->int_data) {
                    ret = false;
                    break;
                }
            } else if (nn->car->typeflag == FLOAT) {
                if (n->car->float_data < nn->car->float_data) {
                    ret = false;
                    break;
                }
            } else {
                ret = false;
                break;
            }
        } else {
            ret = false;
            break;
        }
        n = nn;
        nn = nn->cdr;
    }

    freeConsCells (d->cons);
    d->typeflag = BOOL;
    d->cons = NULL;
    if (ret == true) {
        d->bool = true;
    } else {
        d->bool = false;
    }
    return true;
}
void mystrcpy (char *from, char *to) {
    while ((*to = *from) != '\0') {
        to++;
        from++;
    }
}
bool initDefinePool_addBool () {
    int i,r;
    struct Cons *c, *n = DefinePool->cons;;
    struct Data *d;
/*   "true"   true  "false"  false    NIL
  D -> [][] -> [][] -> [][] -> [][] -> [][/]
       (3)     (2)      (1)    (0)
*/
    for (i = 0; i < 4; i++) {
        if ((r = getConsCell ()) == MAXBUF) {
            return false;
        }
        c = &ConsCells[r];
        c->cdr = n;
        if ((r = getData ()) == MAXBUF) {
            return false;
        }
        d = &Datas[r];
        c->car = d;
        n = c;
        if (i == 0) {
            d->typeflag = BOOL;
            d->bool = false;
        } else if (i == 1) {
            d->typeflag = SYMBOL;
            mystrcpy ("false",d->char_data);
        } else if (i == 2) {
            d->typeflag = BOOL;
            d->bool = true;
        } else if (i == 3) {
            d->typeflag = SYMBOL;
            mystrcpy ("true",d->char_data);
        } 
    }
    DefinePool->cons = n;
    return true;
}
bool initDefinePood() {
    int i;
    bool ret = false;
    if (DefinePool == NULL) {
        ret = true;
        if ((i = getData ()) == MAXBUF) {
            return false;
        }
        DefinePool = & Datas[i];
        DefinePool->typeflag = CONS;
        if ((i = getConsCell ()) == MAXBUF) {
            return false;
        }
        DefinePool->cons = &ConsCells[i];
        if ((i = getData ()) == MAXBUF) {
            return false;
        }
        DefinePool->cons->car = &Datas[i];
        DefinePool->cons->car->typeflag = NIL;
        DefinePool->cons->cdr = NULL;
    }

    /* add TRUE, FALSE */
    initDefinePool_addBool ();

    return ret;
}
bool BI_define (struct Data *d) {
    int i;
    bool ret = true;
    /* DefinePool */
    /*
         (define   <key>   <val>)  NIL
     *d -> [][] -> [][] -> [][] -> [][]
    */
    freeData (d->cons->cdr->cdr->cdr->car);
    freeConsCell (d->cons->cdr->cdr->cdr);

    d->cons->cdr->cdr->cdr = DefinePool->cons;
    DefinePool->cons = d->cons->cdr;

    freeData (d->cons->car);
    freeConsCell (d->cons);
    copyData (DefinePool->cons->car, d);

    return ret;
}
bool BI_lambda_helper (struct Data *d) {
    bool ret = true;
    int i;
    struct Cons *tmp = d->cons->car->cons->cdr;
    struct Data *params = tmp->car,
                *body = tmp->cdr->car,
                args;
                args.cons = d->cons->cdr;
                args.typeflag = CONS;

    i = setDefinePoolS (params, &args);
    if (i == 0) {
        ret = false;
    } else {
        tmp = d->cons;
        d->cons = body->cons;
        freeConsCell (tmp->car->cons->cdr->cdr);
        freeConsCell (tmp->car->cons->cdr);
        freeData (tmp->car->cons->car);
        freeConsCell (tmp->car->cons);
        freeData (tmp->car);
        freeConsCell (tmp);
        evalS (d);
        freeDefinePoolS (i);
    }
    return ret;
}
bool BI_lambda (struct Data *d) {
    bool ret = true,f;
    struct Data *params = d->cons->car->cons->cdr->car,
                *body = d->cons->car->cons->cdr->cdr->car;

    if (((f = is_list(params)) == true) &&
        ((f = is_list(body)) == true)) { 
        ret = BI_lambda_helper (d);
    } else {
        ret = false;
    }
    return ret;
}
bool BI_loop (struct Data *d) {
    return true;
}
bool BI_load (struct Data *d) {
    char * file_name;
    FILE * fd;

    if (d->cons->cdr->car->typeflag == STRING) {
        file_name = d->cons->cdr->car->char_data;
        fd = fopen(file_name, "r");
        if (fd == NULL) {
            printf("file open NG.\n");
            return false;
        }
        printf("Now implementing...\n");
    } else {
        return false;
    }
    fclose(fd);
    return true;
}
bool BI_quote (struct Data *d) {
    bool f;
    struct Cons *tmp = d->cons;
    if ((f = is_list (d->cons->cdr->car)) == true) {
        tmp = d->cons->cdr->car->cons;
        d->cons->cdr->car->cons = NULL;
        d->cons->cdr->car->typeflag = INT;
        freeConsCells (d->cons);
        d->cons = tmp;
    } else {
        copyData (d->cons->cdr->car, d);
        freeConsCells (tmp->cdr);
        freeData (tmp->car);
        freeConsCell (tmp);
    }
    return true;
}
bool BI_list (struct Data *d) {
    bool f;
    struct Cons *tmp = d->cons;

    d->cons = d->cons->cdr;

    tmp->cdr = NULL;
    freeConsCells (tmp);

    return true;
}
bool BI_car (struct Data *d){
    bool ret = true,f;
/*   car     <a>     <b>
             [][] -> [][]
  d->[][] -> [][]
*/
    struct Cons *tmp1,*tmp2;
    if (((f = is_list (d)) == true) &&
        ((f = is_list (d->cons->cdr->car)) == true)) {
        tmp1 = d->cons->cdr->car->cons;
        tmp2 = d->cons;
        copyData (tmp1->car, d);
        tmp1->car->typeflag = INT;
        tmp1->car->cons = NULL;
        freeConsCells (tmp2);
    } else {
        ret = false;
    }
    return ret;
}
bool BI_cdr (struct Data *d) {
    bool ret = true,f;
    struct Cons *tmp;
    if ((f = is_list (d->cons->cdr->car)) == true) {
        tmp = d->cons;
        d->cons = d->cons->cdr->car->cons->cdr;
        tmp->cdr->car->cons->cdr = NULL;
        freeConsCells (tmp);
        if ((d->cons->car->typeflag == NIL) && (d->cons->cdr == NULL)) {
            /* set NIL */
            freeConsCell (d->cons);
            d->typeflag = NIL;
        }
    } else if (d->cons->cdr->car->typeflag == NIL) {
        tmp = d->cons;
        copyData (d->cons->cdr->car, d);
        freeConsCells (tmp);
    } else {
        ret = false;
    }
    return ret;
}
bool BI_cons (struct Data *d) {
    int i;
    struct Cons *tmp;
    struct Data *car = d->cons->cdr->car, *cdr = d->cons->cdr->cdr->car;
    bool ret = true, f;

    /* parameter chech */
    if ((d->cons->cdr->cdr == NULL) || (d->cons->cdr->cdr->cdr == NULL)) {
        /* -- 0/1 arg => NG. */
        ret = false;
        return ret;
    } else if ((d->cons->cdr->cdr->cdr->car->typeflag == NIL) && (d->cons->cdr->cdr->cdr->cdr == NULL)){
        /* -- 2 args => OK. */
    } else {
        /* -- other => NG. */
        printf ("[cons] more parameter\n");
        ret = false;
        return ret;
    }

    if ((f = is_list (cdr)) == true) {
        if (((f = is_atom (car)) == true) || 
            ((f = is_list (car)) == true)) {
            /* ----------------------------------- */
            /* (cons <atom> (list)) => (atom list) */
            /* (cons (list) (list)) => ((list) list) */
            /* ----------------------------------- */
            tmp = d->cons; 
            d->cons = d->cons->cdr;
            freeData (tmp->car);
            freeConsCell (tmp);
            tmp = d->cons->cdr;
            d->cons->cdr = cdr->cons;
            freeConsCell (tmp);
        } else {
            ret = false;
        }
    } else if (cdr->typeflag == NIL) {
        if (((f = is_atom (car)) == true) ||
            ((f = is_list (car)) == true)) {
            /* --------------------------- */
            /* (cons <atom> NIL) => (atom) */
            /* (cons (list) NIL) => ((list)) */
            /* --------------------------- */
            tmp = d->cons->cdr->cdr; 
            d->cons->cdr->cdr = d->cons->cdr->cdr->cdr;
            freeData (tmp->car);
            freeConsCell (tmp);
            tmp = d->cons; 
            d->cons = d->cons->cdr;
            freeData (tmp->car);
            freeConsCell (tmp);
        } else {
            ret = false;
        }
    } else if ((f = is_atom (cdr)) == true) {
            /* Dot pair */
            ret = false;
    } else {
        ret = false;
    }

    return ret;
}
int setDefinePoolS (struct Data *k, struct Data *v){
    int ret = 0;
    struct Cons *nk = k->cons, *nv = v->cons, *tmp1, *tmp2;
    while ((nk->cdr != NULL) && (nk->car->typeflag != NIL)) {
        tmp1 = DefinePool->cons;
        tmp2 = nk->cdr;
        DefinePool->cons = nk;
        nk = tmp2;
        tmp2 = nv->cdr;
        DefinePool->cons->cdr = nv;
        nv = tmp2;
        DefinePool->cons->cdr->cdr = tmp1;
        ret++;
    }
    if ((nk == NULL ) && (nv != NULL)){ 
        ret = 0;
    } else if ((nk != NULL) && (nv == NULL)) {
        ret = 0;
    } else {
        freeData (k);
        freeConsCells(nk);
        freeConsCells(nv);
    }
    return ret;
}
void freeDefinePoolS (int size) {
    int i;
    struct Cons *tmp;
    for (i=0;i<size;i++) {
        tmp = DefinePool->cons;
        DefinePool->cons = DefinePool->cons->cdr->cdr;
        tmp->cdr->cdr = NULL;
        freeConsCells (tmp);
    }
}
bool BI_minus_float (struct Data *d) {
    bool ret = true;
    float ret_float;
    struct Cons *args = d->cons->cdr;

    if (args->car->typeflag == FLOAT) {
        ret_float = args->car->float_data;
    } else {
        ret_float = (float)args->car->int_data;
    }
    args = args->cdr;

    while (args->cdr != NULL) {
        if (args->car->typeflag == FLOAT) {
            ret_float -= args->car->float_data;
        } else {
            ret_float -= (float)args->car->float_data;
        }
        args = args->cdr;
    }

    freeConsCells (d->cons);
    d->typeflag = FLOAT;
    d->float_data = ret_float;
    d->cons = NULL;
    return ret;
}
bool BI_minus_int (struct Data *d) {
    bool ret = true;
    struct Cons *args = d->cons->cdr;
    int ret_int = args->car->int_data;

    args = args->cdr;

    while (args->cdr != NULL) {
        ret_int -= args->car->int_data;
        args = args->cdr;
    }

    freeConsCells (d->cons);
    d->typeflag = INT;
    d->int_data = ret_int;
    d->cons = NULL;
    return ret;
}

bool BI_minus (struct Data *d) {
    bool ret = true, isfloat = false;
    struct Cons *args = d->cons->cdr;

    /* all int or not */
    while (args->cdr != NULL) {
        if (args->car->typeflag == FLOAT) {
            isfloat = true;
            break;
        }
        args = args->cdr;
    }

    if (isfloat == true) {
        ret = BI_minus_float (d);
    } else {
        ret = BI_minus_int(d);
    }
    return ret;
}
bool BI_mult_float (struct Data *d) {
    bool ret = true;
    float result = 1.0;
    struct Cons * args = d->cons->cdr;

    while (args->cdr != NULL) {
        if (args->car->typeflag == FLOAT) {
            result *= args->car->float_data;
        } else {
            result *= (float)args->car->int_data;
        }
        args = args->cdr;
    }
    freeConsCells (d->cons);
    d->typeflag = FLOAT;
    d->float_data = result;
    d->cons = NULL;
    return ret;
}
bool BI_mult_int (struct Data *d) {
    bool ret = true;;
    int result = 1;
    struct Cons *args = d->cons->cdr;

    while (args->cdr != NULL) {
        result = result * (args->car->int_data);
        args = args->cdr;
    }

    freeConsCells (d->cons);
    d->typeflag = INT;
    d->int_data = result;
    d->cons = NULL;
    return ret;
}
bool BI_mult (struct Data *d) {
    bool ret = true, isfloat = false;
    struct Cons *args = d->cons->cdr;
    while (args->cdr != NULL) {
        if (args->car->typeflag == FLOAT) {
            isfloat = true;
            break;
        }
        args = args->cdr;
    }
    if (isfloat == true) {
        ret = BI_mult_float (d);
    } else {
        ret = BI_mult_int (d);
    }
    return ret;
}
bool BI_set (struct Data *d) {
    bool ret = false,f;
    struct Data *k = d->cons->cdr->car,
                *v = d->cons->cdr->cdr->car;
    struct Cons *c = DefinePool->cons;

    if ((f = is_atom(v)) == true) {
        while ((c->cdr != NULL) && (c->car->typeflag != NIL)) {
            if ((c->car->typeflag == SYMBOL) && (compString(k->char_data,c->car->char_data) == true)) {
                ret = true;
                copyData (v, c->cdr->car);
                c = d->cons;
                copyData (k, d);
                freeConsCells (c);
                break;
            }
            c = c->cdr->cdr;
        }
    }
    return ret;
}
bool eval_progn (struct Data *d) {
    bool ret = true, f;
    /*   <p1>  <p2>  NIL
      d->[][]->[][]->[][]
     */
    struct Cons *p = d->cons->cdr;
    struct Cons *tmp;
    struct Data c;
    copyData (d, &c);

    while ((p->car->typeflag != NIL) && (p->cdr != NULL)) {
        if ((f = is_list(p->car)) == true) {
            evalEach (p->car);
        } else {
            evalAtom (p->car);
        }
        tmp = p;
        p = p->cdr;
    }
    if ((f = is_list (tmp->car)) == true) {
        d->cons = tmp->car->cons;
        tmp->car->typeflag = INT;
        tmp->car->cons = NULL;
    } else {
        copyData (tmp->car, d);
    }
    freeConsCells (c.cons);
    return ret;
}
int eval_let_helper_set_DefinePools(struct Data *d) {
    struct Cons *p = d->cons, *tmp;
    int ret = 0;

    while ((p->car->typeflag != NIL) && (p->cdr != NULL)) {
        tmp = DefinePool->cons;
        DefinePool->cons = p->car->cons;
        freeConsCells(p->car->cons->cdr->cdr);
        p->car->cons = NULL;
        p->car->typeflag = INT;
        DefinePool->cons->cdr->cdr = tmp;
        p = p->cdr;
        ret++;
    }
    return ret;
}
void eval_let_helper_free_DefinePools(int i) {
    freeDefinePoolS (i);
}
bool eval_let (struct Data *d) {
    bool ret = true, f;
    int i;
    struct Data *let = d->cons->cdr->car;
    struct Data *body = d->cons->cdr->cdr->car;

    /* set DefinePools */
    i = eval_let_helper_set_DefinePools(let);

    /* eval body */
    if ((f = is_list (body)) == true) {
        ret = evalS(body);
        if (ret == false) {
            return false;
        }
    } else {
        ret = evalAtom(body);
        if (ret == false) {
            return false;
        }
    }
    if ((f = is_list (body)) == true) {
        ret = evalS(body);
        if (ret == false) {
            return false;
        }
    } else {
        ret = evalAtom(body);
        if (ret == false) {
            return false;
        }
    }

    /* free DefinePools */
    eval_let_helper_free_DefinePools(i);

    /* free data and set body to *d */
    d->cons->cdr->cdr->car = NULL;
    freeConsCells(d->cons);
    copyData (body, d);
    return ret;
}
bool BI_atom (struct Data *d) {
    bool ret = true, flag;

    flag = is_atom (d->cons->cdr->car);
    freeConsCells (d->cons);
    d->cons = NULL;
    d->typeflag = BOOL;
    d->bool = flag;
    return ret;
}
bool is_atom (struct Data *d) {
    bool ret = true;
    if (d->typeflag == CONS) {
        ret = false;
    }

    return ret;
}
bool is_list (struct Data *d) {
    bool ret = false;
    if (d->typeflag == CONS) {
        ret = true;
    }
    return ret;
}
