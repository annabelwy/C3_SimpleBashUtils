#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    struct myFlags flag = {0};
    flag.patterns = (char **)malloc(100 * sizeof(char *));
    flag.name_of_file = (char **)malloc(100 * sizeof(char *));
    flag.files_for_f = (char **)malloc(
        100 * sizeof(char *));  // было сайз оф чар без звездочки
    for (int i = 0; i < 100; i++) {
      flag.patterns[i] = (char *)malloc(2048 * sizeof(char));
      flag.name_of_file[i] = (char *)malloc(2048 * sizeof(char));
      flag.files_for_f[i] = (char *)malloc(2048 * sizeof(char));
      flag.patterns[i][0] = '\0';
      flag.name_of_file[i][0] = '\0';
      flag.files_for_f[i][0] = '\0';
    }
    parser(argc, argv, &flag);
    flazhki(&flag);
    for (int i = 0; i < 100; i++) {
      free(flag.patterns[i]);
      free(flag.name_of_file[i]);
      free(flag.files_for_f[i]);
    }
    free(flag.patterns);
    free(flag.name_of_file);
    free(flag.files_for_f);
  }
}

int count_of_coincidences(struct myFlags *flag, char **str_file, int length,
                          int *number_string) {  // выводит сколько сопадений
  size_t nmatch = 1;
  regmatch_t pmatch[1];
  int count_of_strings = 0;
  for (int i = 0; i < length; i++) {  //по массиву строк из файла
    int flag_v = 0;
    for (int j = 0; j < flag->count_of_patterns; j++) {
      if (regexec(&flag->preg[j], str_file[i], nmatch, pmatch, 0) == 0 &&
          flag->flag_v == 0) {
        number_string[count_of_strings] =
            i + 1;  // номер строки из файла, в которой нашли совпадение
        count_of_strings++;
        j = flag->count_of_patterns;
      } else if (regexec(&flag->preg[j], str_file[i], nmatch, pmatch, 0) != 0 &&
                 flag->flag_v == 1) {
        flag_v++;
        if (flag->count_of_patterns ==
            flag_v) {  //если все флаги прошли через В
          number_string[count_of_strings] = i + 1;
          count_of_strings++;
          j = flag->count_of_patterns;
        }
      }
    }
  }
  return count_of_strings;
}

void patterns_from_f(struct myFlags *flags) {
  int len;
  for (int i = 0; i < flags->count_of_f; i++) {
    FILE *text = fopen(flags->files_for_f[i], "r");
    if (text != NULL) {
      char str[2048] = "\0";
      while (fgets(str, sizeof(str), text) != NULL) {
        strcat(flags->patterns[flags->count_of_patterns], str);
        len = strlen(str);
        if (flags->patterns[flags->count_of_patterns][len - 1] == '\n' &&
            len > 1) {
          flags->patterns[flags->count_of_patterns][len - 1] = '\0';
        }
        flags->count_of_patterns++;
        str[0] = '\0';
      }
      fclose(text);
    } else if (flags->flag_s == 0) {
      fprintf(stderr, "grep: %s: No such file or directory\n",
              flags->name_of_file[i]);
    }
  }
}
void flazhki(struct myFlags *flags) {
  patterns_from_f(flags);  //  записываю шаблоны из ф
  string_to_regex(flags);
  for (int i = 0; i < flags->count_of_names; i++) {  // цикл по названиям файлов
    int n = 0;
    FILE *text = fopen(flags->name_of_file[i], "r");
    char **str = NULL;
    if (text) {
      int number_string_array[10000];
      while (!feof(text)) {
        str = (char **)realloc(str, (sizeof(char *) * (n + 1)));
        str[n] = NULL;  //увеличиваем количество символов в n-ой строке
        char ch;
        int x = 0;
        while ((ch = fgetc(text)) != '\n' && ch != EOF) {
          str[n] = (char *)realloc(str[n], ((x + 1) * sizeof(char)));
          str[n][x++] = ch;
        }
        if (ch != EOF || (ch == EOF && x > 0)) {
          str[n] = (char *)realloc(str[n], ((x + 2) * sizeof(char)));
          str[n][x] = '\n';
          str[n][x + 1] = '\0';
          n++;
        }
      }
      fclose(text);
      int number_of_rows =
          count_of_coincidences(flags, str, n, number_string_array);
      if (flags->flag_c == 1) {
        if (flags->flag_l && number_of_rows > 1) {
          number_of_rows = 1;
        }
        if (flags->flag_h == 0 && flags->count_of_names > 1) {
          printf("%s:%d\n", flags->name_of_file[i], number_of_rows);
        } else {
          printf("%d\n", number_of_rows);  // если флаг h, то без названия файла
        }
      } else if (flags->flag_l != 1) {
        for (int j = 0; j < number_of_rows; j++) {
          if (flags->flag_h == 0 && flags->count_of_names > 1) {
            printf("%s:", flags->name_of_file[i]);
          }
          if (flags->flag_n == 1) {
            printf("%d:", number_string_array[j]);
          }

          if (flags->flag_o == 1 && flags->flag_v == 0) {
            int start = 0;  //по строкам
            regmatch_t pmatch[1];
            for (int k = 0; k < flags->count_of_patterns;
                 k++) {  //  по шаблонам
              while (regexec(&flags->preg[k],
                             &str[number_string_array[j] - 1][start], 1, pmatch,
                             0) == 0) {
                for (int m = pmatch[0].rm_so; m < pmatch[0].rm_eo; m++) {
                  printf("%c",
                         str[number_string_array[j] - 1]
                            [m + start]);  // меняем относительно первой строки
                }
                printf("\n");
                start += pmatch[0].rm_so + 1;
              }
            }
          } else {
            printf("%s", str[number_string_array[j] - 1]);
          }
        }
      }

      if (flags->flag_l == 1) {
        if (number_of_rows > 0) {
          printf("%s\n", flags->name_of_file[i]);  // если строчки есть, то
                                                   // вывожу название файла
        }
      }
      for (int k = 0; k < n; k++) {
        free(str[k]);
      }
    } else if (flags->flag_s == 0) {
      fprintf(stderr, "grep: %s: No such file or directory\n",
              flags->name_of_file[i]);
    }
    free(str);
  }
  for (int i = 0; i < flags->count_of_patterns; i++) {
    regfree(&flags->preg[i]);
  }
}

void string_to_regex(struct myFlags *flags) {
  int flags_i = REG_EXTENDED;
  if (flags->flag_i) {
    flags_i = REG_ICASE;
  }
  for (int i = 0; i < flags->count_of_patterns; i++) {
    regcomp(&flags->preg[i], flags->patterns[i],
            flags_i);  //заполнили преги(массив масок)
  }
}

int parser_ef(int argc, char **argv) {
  int count = 0;
  for (int i = 1; i < argc && count == 0; i++) {
    int len;
    len = strlen(argv[i]);
    if (argv[i][0] == '-') {
      for (int j = 1; j < len; j++) {
        if (argv[i][j] == 'e' || argv[i][j] == 'f') {
          count++;
        }
      }
    }
  }
  return count;
}

void parser(int argc, char **argv, struct myFlags *flags) {
  int len;
  int count = 0;  // стоит закомментить?
  int flag_ef = parser_ef(argc, argv);
  for (int i = 1; i < argc; i++) {
    len = strlen(argv[i]);
    for (int j = 0; j < len; j++) {
      if (argv[i][0] == '-') {
        if (argv[i][j] == 'e') {
          flags->flag_e = 1;
          if (j < len - 1) {
            strcat(flags->patterns[flags->count_of_patterns],
                   &argv[i][j + 1]);  // j-e, след-но с j+1
            flags->count_of_patterns++;
            j = len;
          } else {
            strcat(flags->patterns[flags->count_of_patterns], argv[i + 1]);
            flags->count_of_patterns++;
            i++;
            j = len;
          }
        } else if (argv[i][j] == 'i') {
          flags->flag_i = 1;
        } else if (argv[i][j] == 'v') {
          flags->flag_v = 1;
        } else if (argv[i][j] == 'c') {
          flags->flag_c = 1;
        } else if (argv[i][j] == 'l') {
          flags->flag_l = 1;
        } else if (argv[i][j] == 'n') {
          flags->flag_n = 1;
        } else if (argv[i][j] == 'h') {
          flags->flag_h = 1;
        } else if (argv[i][j] == 'o') {
          flags->flag_o = 1;
        } else if (argv[i][j] == 's') {
          flags->flag_s = 1;
        } else if (argv[i][j] == 'f') {
          flags->flag_f = 1;
          if (j < len - 1) {
            strcat(flags->files_for_f[flags->count_of_f],
                   &argv[i][j + 1]);  // правильно ли?
            flags->count_of_f++;
          } else {
            strcat(flags->files_for_f[flags->count_of_f], argv[i + 1]);
            flags->count_of_f++;
            i++;
          }
          j = len;  // выходим после отработки флага ф
        }
      } else if (count == 0 && flag_ef == 0) {
        strcat(flags->patterns[flags->count_of_patterns], argv[i]);
        flags->count_of_patterns++;
        j = len;
        count++;
      } else {
        strcat(flags->name_of_file[flags->count_of_names], argv[i]);
        flags->count_of_names++;
        j = len;
      }
    }
  }
}