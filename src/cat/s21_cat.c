#include "s21_cat.h"

int main(const int argc, const char **argv) {
  FILE *fl = NULL;
  enabledOptions opt;
  optInit(&opt);
  int i = 1;
  while (i < argc && (argv[i][0] == '-') && opt.result) {
    opt = parseOpt(argv[i] + 1);
    i++;
  }
  if (opt.result == 0) {
    illegalOptionErrorPrint(opt.err);
  }
  while (i < argc && argv[i] && opt.result) {
    if (checkFile((argv[i]), &fl, 1)) {
      printFile(&opt, fl);
    }
    i++;
  }
  return 0;
}

void optInit(enabledOptions *opt) {
  opt->bOpt = 0;
  opt->eOpt = 0;
  opt->nOpt = 0;
  opt->sOpt = 0;
  opt->tOpt = 0;
  opt->vOpt = 0;
  opt->eGnuOpt = 0;
  opt->tGnuOpt = 0;
  opt->err = ' ';
  opt->result = 1;
}

enabledOptions parseOpt(const char *str) {
  enabledOptions tmp;
  optInit(&tmp);
  int gnuFlag = 1;
  while (*str && tmp.result && gnuFlag) {
    switch (*str) {
      case 'b':
        tmp.bOpt = 1;
        break;
      case 'e':
        tmp.eOpt = 1;
        break;
      case 'E':
        tmp.eGnuOpt = 1;
        break;
      case 'n':
        tmp.nOpt = 1;
        break;
      case 's':
        tmp.sOpt = 1;
        break;
      case 't':
        tmp.tOpt = 1;
        break;
      case 'T':
        tmp.tGnuOpt = 1;
        break;
      case 'v':
        tmp.vOpt = 1;
        break;
      default:
        if (strcmp(str, "-number-nonblank") == 0) {
          tmp.bOpt = 1;
        } else if (strcmp(str, "-number") == 0) {
          tmp.nOpt = 1;
        } else if (strcmp(str, "-squeeze-blank") == 0) {
          tmp.nOpt = 1;
        } else {
          tmp.result = 0;
          tmp.err = *str;
        }
        gnuFlag = 0;
    }
    str++;
  }
  return tmp;
}

int illegalOptionErrorPrint(char ch) {
  fprintf(stderr,
          "cat: illegal option -- %c\nusage: cat [-benstv] [file ...]\n", ch);
  return 0;
}

int printFile(enabledOptions *opt, FILE *fl) {
  char *line = NULL;
  size_t len = 0;
  ssize_t lineLength;
  int lineNumber = 0, isLineEmpty = 0, isLineEmptyAgain = 0;
  while ((lineLength = getline(&line, &len, fl)) != -1) {
    lineNumber++;
    if (lineLength < 2) {
      if (isLineEmpty) {
        isLineEmptyAgain = 1;
      }
      isLineEmpty = 1;
    } else {
      isLineEmpty = 0;
      isLineEmptyAgain = 0;
    }
    printLine(&line, &lineNumber, isLineEmpty, isLineEmptyAgain, opt,
              (int)lineLength);
  }
  free(line);
  fclose(fl);
  fflush(stdout);
  return 0;
}

void printLine(char **line, int *lineNumber, int isLineEmpty,
               int isLineEmptyAgain, enabledOptions *opt, int lineLength) {
  if (opt->bOpt == 1) {
    handleB(line, lineNumber, lineLength, isLineEmpty);
  } else if (opt->eOpt == 1 || opt->eGnuOpt) {
    handleE(line, opt, lineLength);
  } else if (opt->nOpt == 1) {
    fprintf(stdout, "%6d\t", *lineNumber);
    printer(line, lineLength);
  } else if (opt->sOpt == 1) {
    if (!isLineEmptyAgain) {
      printer(line, lineLength);
    }
  } else if (opt->tOpt == 1) {
    handleT(line, opt, lineLength);
  } else if (opt->vOpt == 1) {
    handleV(line, lineLength);
  } else {
    printer(line, lineLength);
  }
}

void handleE(char **line, enabledOptions *opt, int lineLength) {
  char *tmp = *line;
  int charN = 0;
  while (*tmp != '\0' || charN < lineLength) {
    if (*tmp == '\n') {
      fprintf(stdout, "$\n");
    } else {
      if (opt->eGnuOpt) {
        fprintf(stdout, "%c", *tmp);
      } else {
        specialCharPrint(*tmp);
      }
    }
    tmp++;
    charN++;
  }
}

void handleV(char **line, int lineLength) {
  char *tmp = *line;
  int charN = 0;
  while (*tmp != '\0' || charN < lineLength) {
    specialCharPrint(*tmp);
    tmp++;
    charN++;
  }
}

void handleT(char **line, enabledOptions *opt, int lineLength) {
  char *tmp = *line;
  int charN = 0;
  while (*tmp != '\0' || charN < lineLength) {
    if (*tmp == '\t') {
      fprintf(stdout, "^I");
    } else {
      if (opt->tGnuOpt) {
        fprintf(stdout, "%c", *tmp);
      } else {
        specialCharPrint(*tmp);
      }
    }
    tmp++;
    charN++;
  }
}

void specialCharPrint(char ch) {
  int c = (int)ch;

  if (c < 0) {
    ch &= 127;  //  убираем знак
    c = (int)ch;
    c += 128;
  }

  if (c > 127 && c < 160) {
    c -= 64;
    fprintf(stdout, "M-^");
  } else if (ch < 32 && ch != 9 && ch != 10) {
    c += 64;
    fprintf(stdout, "^");
  } else if (ch == 127) {
    c -= 64;
    fprintf(stdout, "^");
  }
  fprintf(stdout, "%c", c);
}

void handleB(char **line, int *lineNumber, int lineLength,
             int lineEmptyStatus) {
  if (lineEmptyStatus) {
    fprintf(stdout, "\n");
    *lineNumber = *lineNumber - 1;
  } else {
    fprintf(stdout, "%6d\t", *lineNumber);
    printer(line, lineLength);
  }
}

void printer(char **line, int lineLength) {
  char *tmp = *line;
  int charN = 0;
  while (*tmp != '\0' || charN < lineLength) {
    fprintf(stdout, "%c", *tmp);
    tmp++;
    charN++;
  }
}
