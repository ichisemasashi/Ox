#define OK 0
#define NG 1

typedef enum {not_use = 0, use} gc_type;
typedef enum {empty = 0, NIL, SYMBOL, STRINGS, INT, FLOAT, CONS_CELL} kind;
struct cell {
  gc_type gc;
  kind kind;
  void *value;
};
#define Inner_BUF_SIZE 4096
#define Cell_BUF_SIZE Inner_BUF_SIZE
struct cell cell_BUF[Cell_BUF_SIZE];

struct cons_cell {
  gc_type gc;
  struct cell *car;
  struct cons_cell *cdr;
};
#define Cons_Cell_BUF_SIZE Inner_BUF_SIZE
struct cons_cell Cons_Cell_BUF[Cons_Cell_BUF_SIZE];

typedef struct {
  gc_type gc;
  unsigned char str[256];
} i_symbol;
typedef i_symbol i_strings;
typedef struct {
  gc_type gc;
  int i;
} i_int;
typedef struct {
  gc_type gc;
  double f;
} i_float;

i_symbol sym_BUF[Inner_BUF_SIZE];
i_strings str_BUF[Inner_BUF_SIZE];
i_int int_BUF[Inner_BUF_SIZE];
i_float float_BUF[Inner_BUF_SIZE];

void init();
void free_cell(struct cell *);
void free_cons(struct cons_cell *);
void free_cons_perfect(struct cons_cell *);
void free_symbol(i_symbol *);
void free_strings(i_strings *);
void free_int(i_int *);
void free_float(i_float *);
int get_cell(struct cell *);
int get_cons_cell(struct cons_cell *);
int get_symbol(i_symbol *);
int get_strings(i_strings *);
int get_int(i_int *);
int get_float(i_float *);

int my_read();
int my_eval();
int my_print();
void prompter(char *);
int read_from(FILE *, unsigned char *, int);
int parse_input(unsigned char *, int);
