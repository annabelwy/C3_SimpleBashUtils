#ifndef S21_CAT_H_
#define S21_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct myFlags {
  int flag_b;
  int flag_e;
  int flag_n;
  int flag_s;
  int flag_t;
  int flag_v;
};
void functions(char *name_file, struct myFlags flags);
int parser(int argc, char **argv, struct myFlags *flags);

#endif  // S21_CAT_H_