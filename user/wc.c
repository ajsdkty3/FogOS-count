#include "kernel/types.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int count_punct = 0, count_digits = 0, count_upper = 0, count_lower = 0;
int flags_specified = 0;

char buf[512];

int is_digit(char c) {
  return c >= '0' && c <= '9';
}

int is_uppercase(char c) {
  return c >= 'A' && c <= 'Z';
}

int is_lowercase(char c) {
  return c >= 'a' && c <= 'z';
}

int is_punctuation(char c) {
  return c == '.' || c == ',' || c == '!' || c == '?' || c == ';' || c == ':' ||
         c == '\'' || c == '"' || c == '(' || c == ')' || c == '[' || c == ']' ||
         c == '{' || c == '}' || c == '-' || c == '_';
}

/**
 * Counts the number of lines, words, and characters from the file descriptor `fd`.
 * Or counts punctuation marks, digits, uppercase, and lowercase letters,
 * based on flags.
 *
 * Flags:
 * - `-p` : Count punctuation marks.
 * - `-d` : Count digits.
 * - `-u` : Count uppercase letters.
 * - `-l` : Count lowercase letters.
 *
 * @param fd the file descriptor to read from
 * @param name the name of the file being processed
 */
void wc(int fd, char *name) {
  int i, n;
  int l, w, c, inword;
  int punct_count = 0, digit_count = 0, upper_count = 0, lower_count = 0;

  l = w = c = 0;
  inword = 0;

  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
      c++;
      if (buf[i] == '\n')
        l++;
      if (strchr(" \r\t\n\v", buf[i]))
        inword = 0;
      else if (!inword) {
        w++;
        inword = 1;
      }

      if (count_punct && is_punctuation(buf[i])) {
        punct_count++;
      }
      if (count_digits && is_digit(buf[i])) {
        digit_count++;
      }
      if (count_upper && is_uppercase(buf[i])) {
        upper_count++;
      }
      if (count_lower && is_lowercase(buf[i])) {
        lower_count++;
      }
    }
  }

  if (n < 0) {
    printf("wc: read error\n");
    exit(1);
  }

  if (!flags_specified && name[0] != '\0') {
    printf("%d %d %d %s\n", l, w, c, name);
  }

  if (count_punct) {
    printf("Punctuation: %d\n", punct_count);
  }
  if (count_digits) {
    printf("Digits: %d\n", digit_count);
  }
  if (count_upper) {
    printf("Uppercase: %d\n", upper_count);
  }
  if (count_lower) {
    printf("Lowercase: %d\n", lower_count);
  }
}

int main(int argc, char *argv[]) {
  int fd, i;
  
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-p") == 0) {
      count_punct = 1;
      flags_specified = 1;
    } else if (strcmp(argv[i], "-d") == 0) {
      count_digits = 1;
      flags_specified = 1;
    } else if (strcmp(argv[i], "-u") == 0) {
      count_upper = 1;
      flags_specified = 1;
    } else if (strcmp(argv[i], "-l") == 0) {
      count_lower = 1;
      flags_specified = 1;
    } else if (argv[i][0] == '-') {
          printf("wc: invalid option %s\n", argv[i]);
          exit(1);
        } else {
          if ((fd = open(argv[i], 0)) < 0) {
            printf("wc: cannot open %s\n", argv[i]);
            exit(1);
          }
          wc(fd, argv[i]);
          close(fd);
        }
      }

  exit(0);
}
