#include "s21_cat.h"

// FILE *acceptance(char *name_file);

int main(int argc, char **argv)
{
  struct myFlags flag = {0, 0, 0,
                         0, 0, 0}; // если не один и не два, то ноль же
  // flag = {0,0,0,0,0};
  if (parser(argc, argv, &flag) && argc > 1)
  {
    if ((flag.flag_b == 1 || flag.flag_e == 1 || flag.flag_n == 1 ||
         flag.flag_s == 1 || flag.flag_t == 1) &&
        (flag.flag_b == 2 || flag.flag_e == 2 || flag.flag_n == 2 ||
         flag.flag_s == 2 || flag.flag_t == 2))
    {
      return 1; // если и для линукса и для мака - что делать?
    }
    else
    {
      if ((flag.flag_b == 1 && flag.flag_n == 1) ||
          (flag.flag_b == 2 && flag.flag_n == 2))
      {
        flag.flag_n = 0;
      }
      if (flag.flag_e == 1 || flag.flag_t == 1)
      {
        flag.flag_v = 1; // 	тут не переделывается
      }
      // printf("\t%d %d %d %d %d %d\n", flag.flag_b, flag.flag_e, flag.flag_n,
      // flag.flag_s, flag.flag_t, flag.flag_v);
      for (int i = 1; i < argc; i++)
      {
        if (argv[i][0] != '-')
        {
          functions(argv[i], flag);
        }
      }
    }
  }
  return 0;
}

void functions(char *name_file, struct myFlags flags)
{
  FILE *text;
  text = fopen(name_file, "r");
  // FILE *text = acceptance(name_file); // файл открывается через указатель,
  // потому что можем прочитать или изменить
  int k = 0;
  int number = 1;
  char *str = malloc(1 * sizeof(char));
  char ch;
  if (text != NULL)
  {
    while ((ch = fgetc(text)) != EOF)
    {
      k = k + 1;
      str = realloc(str, k * sizeof(char));
      str[k - 1] = ch;
    }
    // printf("\n%s\n\n", str);
    // k = strlen(str);
    for (int i = 0; i < k; i++)
    {
      if ((flags.flag_s == 1 || flags.flag_s == 2) && str[i] == '\n' &&
          str[i + 1] == '\n' && str[i + 2] == '\n')
      {
        continue; // соединяет смежные пустые строки
      }
      if ((flags.flag_n == 1 || flags.flag_n == 2) && i == 0)
      {
        printf("%6d\t", number);
        number++; // нумерует все выходные строки
      }

      if ((flags.flag_b == 1 || flags.flag_b == 2) && i == 0 &&
          str[0] != '\n' && str[i + 1] != '\n')
      {
        printf("%6d\t", number);
        number++; // нумерует только непустые строки
      }

      if ((flags.flag_e == 1 || flags.flag_e == 2) && str[i] == '\n')
      {
        printf("$"); // отображает символы конца строки как $
      }

      if ((flags.flag_t == 1 || flags.flag_t == 2) && str[i] == '\t')
      {
        printf("^I");
        continue; // отображает табы как ^I
      }

      if (flags.flag_v == 1 &&
          ((str[i] >= 0 && str[i] <= 31 && str[i] != 9 && str[i] != 10) ||
           str[i] == 127))
      {
        if (str[i] == 127)
        {
          str[i] = str[i] - 64;
        }
        else
        {
          str[i] = str[i] + 64;
        }
        printf("^%c", str[i]);
        continue;
      }
      printf("%c", str[i]);
      if ((flags.flag_n == 1 || flags.flag_n == 2) && str[i] == '\n' &&
          i < k - 1)
      {
        printf("%6d\t", number);
        number++;
      }
      if ((flags.flag_b == 1 || flags.flag_b == 2) && str[i] == '\n' &&
          str[i + 1] != '\n' && i < k - 1)
      {
        printf("%6d\t", number);
        number++;
      }
    }
    fclose(text);
  }
  free(str);
}

int parser(int argc, char **argv, struct myFlags *flags)
{
  int res = 1;
  int len;
  for (int i = 1; i < argc - 1; i++)
  { // по элементам
    len = strlen(argv[i]);
    if (argv[i][0] == '-' && argv[i][1] == '-')
    {
      if (strcmp(argv[i], "--number-nonblank") == 0)
      {
        flags->flag_b = 2;
      }
      // else if (argv[i][j] == 'E' ) {
      // 	flags->flag_e = 2;
      // }
      else if (strcmp(argv[i], "--number") == 0)
      {
        flags->flag_n = 2;
      }
      else if (strcmp(argv[i], "--squeeze-blank") == 0)
      {
        flags->flag_s = 2;
      }
      // else if (argv[i][j] == 't') {
      // 	flags->flag_t = 2;
      // }
      else
      {
        res = 0;
        break;
      }
    }
    else if (argv[i][0] == '-')
    {
      for (int j = 1; j < len; j++) // то из чего состоит элемент
      {
        if (argv[i][j] == 'b')
        {
          flags->flag_b = 1;
        }
        else if (argv[i][j] == 'E')
        {
          flags->flag_e = 2;
        }
        else if (argv[i][j] == 'T')
        {
          flags->flag_t = 2;
        }
        else if (argv[i][j] == 'e')
        {
          flags->flag_e = 1;
        }
        else if (argv[i][j] == 'n')
        {
          flags->flag_n = 1;
        }
        else if (argv[i][j] == 's')
        {
          flags->flag_s = 1;
        }
        else if (argv[i][j] == 't')
        {
          flags->flag_t = 1;
        }
        else if (argv[i][j] == 'v')
        {
          flags->flag_v = 1;
        }
        else
        {
          res = 0;
          break;
        }
      }
    }
    else
    {
      break;
    }
  }
  // printf("\nresult: %d\n", res);
  return res;
}
