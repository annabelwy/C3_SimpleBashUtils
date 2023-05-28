#ifndef S21_GREP_H_
#define S21_GREP_H_

#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct myFlags {
  int flag_e, flag_i, flag_v, flag_c, flag_l, flag_n, flag_h, flag_s, flag_f,
      flag_o;
  int count_of_patterns, count_of_names, count_of_f;
  char **patterns;
  char **name_of_file;
  regex_t preg[1000];
  char **files_for_f;
};

void flazhki(struct myFlags *flags);
void string_to_regex(struct myFlags *flags);
void parser(int argc, char **argv, struct myFlags *flags);
int count_of_coincidences(struct myFlags *flag, char **str_file, int length,
                          int *number_string);
void patterns_from_f(struct myFlags *flags);
int parser_ef(int argc, char **argv);

#endif  // S21_GREP_H_