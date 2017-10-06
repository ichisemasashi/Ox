#include <stdio.h>
#include <float.h>

#define MAXBUF 4096 /* buf size */
#define MAXTOKEN 4096 /* token size */
#define MAXMEMORY 4096

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
    return TRUE;
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
int isSymbol (struct token* x) {
    int i;
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
