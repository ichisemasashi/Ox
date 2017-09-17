#include <stdio.h>
#define MAXBUF 4096 /* buf size */
#define MAXTOKEN 4096 /* token size */

static int buf[MAXBUF];
static int parseBuf[MAXBUF];
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
        return 1;
    }
    i = tokenize (buf, MAXBUF, tokens, MAXTOKEN);
    if (i > MAXTOKEN) {
        return 1;
    }
    putToken();
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
        if (ret != 0) {
            printf("ERROR!!!!\n");
            break;
        }
        myEval();
        myPrint();
    }
    return 0;
}
