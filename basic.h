#define OK 0
#define NG 1

typedef enum {not_use = 0, use} gc_type;
typedef enum {empty = 0, NIL, SYMBOL, STRINGS, INT, FLOAT, BOOL, CONS_CELL} kind;
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
#define STR_BUF_SIZE 256

struct token {
  unsigned char *tok;
};

/* ------------------- */
/* read from stdin     */
/* ------------------- */
struct cell my_cell;

/* ------------------- */
/* cell for printer    */
/* ------------------- */
struct cell my_printer_cell;

typedef struct {
  gc_type gc;
  unsigned char str[STR_BUF_SIZE];
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
typedef enum {FALSE, TRUE} bool;
typedef struct {
  gc_type gc;
  bool bl;
} i_bool;

i_symbol sym_BUF[Inner_BUF_SIZE];
i_strings str_BUF[Inner_BUF_SIZE];
i_int int_BUF[Inner_BUF_SIZE];
i_float float_BUF[Inner_BUF_SIZE];
i_bool bool_BUF[Inner_BUF_SIZE];


int skip_spaces(unsigned char *);
void init();
void free_cell(struct cell *);
void free_cons(struct cons_cell *);
void free_cons_perfect(struct cons_cell *);
void free_symbol(i_symbol *);
void free_strings(i_strings *);
void free_int(i_int *);
void free_float(i_float *);
void free_bool(i_bool *);
int get_cell(struct cell **);
int get_cons_cell(struct cons_cell **);
int get_prime_symbol(i_symbol **);
int get_prime_strings(i_strings **);
int get_prime_int(i_int **);
int get_prime_float(i_float **);
int get_prime_bool(i_bool **);
int my_sizeof(unsigned char *);

void set_prime_nil(struct cell *);
int set_prime_bool(struct cell *, bool);
int set_prime_strings(struct cell *, unsigned char *, int);
int set_prime_symbol(struct cell *, unsigned char *, int);
int set_prime_int(struct cell *, int);
int set_prime_float(struct cell *, double);

int is_my_hex(char);
int is_my_bin(char);
int is_my_sign(char);
int my_atoi(unsigned char *, int);
double my_atod(unsigned char *, int);

int is_parce_int(unsigned char *, int);
int is_parce_float(unsigned char *, int);
int skip_strings(unsigned char *);
int to_tokenize(unsigned char *, struct token *);
int my_read();
int my_eval();
int eval_cons(struct cell *, struct cell *);
int eval_cons_cell(struct cell *, struct cell *);
void copy_cell(struct cell *, struct cell *);
int my_print();
void print_cell(struct cell *);
void print_cons(struct cell *);
void prompter(char *);
int read_from(FILE *, unsigned char *, int);
int parse_input_1_sexp(struct token *);
