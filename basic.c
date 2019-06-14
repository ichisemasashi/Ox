#include <stdio.h>
#include "basic.h"

int my_read() {
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
