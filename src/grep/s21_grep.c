#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    char patterns[LENGTH] = {0};
    int argFileInd = -1;
    int fileCount = 0;
    int fileNumber = 0;
    enabledOptions opt;
    optInit(&opt);
    parseFlagsPatternsAndFiles(argc, &argv, &opt, patterns, &argFileInd);
    fileCountChecker(argc, &argv, &opt, patterns, argFileInd, &fileCount);
    if (opt.result)
      grepPatternsInFiles(argc, &argv, &opt, patterns, argFileInd, &fileNumber,
                          &fileCount);
    else
      grepUsagePrint();
  } else
    grepUsagePrint();
  return 0;
}

void fileCountChecker(int argc, char ***argv, enabledOptions *opt,
                      char *patterns, int argFileInd, int *fileCount) {
  if (argFileInd == -1 || strlen(patterns) == 0) {
    *fileCount = 0;
    opt->result = 0;
  } else
    *fileCount = fileCounter(argc, argv, argFileInd);
  if (*fileCount == 1) opt->hOpt = 1;
  if (patterns[strlen(patterns) - 1] == '|')
    patterns[strlen(patterns) - 1] = '\0';
}

int fileCounter(int argc, char ***argv, int argFileInd) {
  int fileCount = 0;
  int index = argFileInd;
  while (index < argc) {
    char *filename = (*(*argv + index));
    char *tmp = (*(*argv + index - 1));
    if (!strcmp(tmp, "-e") || !strcmp(tmp, "-f") || (filename[0] == '-')) {
      index++;
    } else {
      fileCount++;
      index++;
    }
  }
  return fileCount;
}

void grepPatternsInFiles(int argc, char ***argv, enabledOptions *opt,
                         char *patterns, int argFileInd, int *fileNumber,
                         int *fileCount) {
  int index = argFileInd;
  regex_t preg;
  int regexStatus;
  if (opt->iOpt)
    regexStatus = regcomp(&preg, patterns, REG_ICASE);
  else
    regexStatus = regcomp(&preg, patterns, REG_EXTENDED);
  while (index < argc && regexStatus == 0) {
    char *filename = *(*argv + index);
    char *tmp = *(*argv + index - 1);
    if (!strcmp(tmp, "-e") || !strcmp(tmp, "-f") || (filename[0] == '-')) {
    } else {
      FILE *fl = NULL;
      int countMatches = 0;
      if (checkFile(filename, &fl, !opt->sOpt)) {
        char *line = NULL;
        size_t len = 0;
        int lineNumber = 0;
        *fileNumber += 1;
        while ((getline(&line, &len, fl)) != -1) {
          lineHandle(filename, line, opt, &lineNumber, &countMatches, &preg);
        }
        free(line);
      } else {
        countMatches = -1;
        *fileCount -= 1;
      }
      if (opt->cOpt && countMatches != -1)
        grepPrintC(opt, filename, countMatches);
      if (opt->lOpt && countMatches > 0) grepPrintL(filename);
      fclose(fl);
    }
    index++;
  }
  if (regexStatus == 0) regfree(&preg);
}

void grepPrintL(char *filename) { fprintf(stdout, "%s\n", filename); }

void grepPrintO(regex_t *preg, char *line, regmatch_t *pmatch, char *filename,
                enabledOptions *opt, int lineNumber) {
  char *ptr = line;
  int err = 0;
  while (err == 0) {
    if (pmatch[0].rm_eo == pmatch[0].rm_so) break;
    fprintf(stdout, "%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
            ptr + pmatch[0].rm_so);
    ptr += pmatch[0].rm_eo;
    err = regexec(preg, ptr, 1, pmatch, REG_NOTBOL);
    if (err == 0) {
      if (!opt->hOpt) fprintf(stdout, "%s:", filename);
      if (opt->nOpt) fprintf(stdout, "%d:", lineNumber);
    }
  }
}

void grepPrintC(enabledOptions *opt, char *filename, int countMatches) {
  if (!opt->hOpt) fprintf(stdout, "%s:", filename);
  if (opt->lOpt && countMatches > 0) countMatches = 1;
  fprintf(stdout, "%d\n", countMatches);
}

void lineHandle(char *filename, char *line, enabledOptions *opt,
                int *lineNumber, int *countMatches, regex_t *preg) {
  int isMatch = 0;
  size_t nmatch = 2;
  regmatch_t pmatch[2];
  *lineNumber += 1;
  if (regexec(preg, line, nmatch, pmatch, REG_NOTEOL) == 0) isMatch = 1;
  if (opt->vOpt) isMatch = !isMatch;
  if (isMatch) {
    *countMatches = *countMatches + 1;
    int lastEl = strlen(line) - 1;
    char lastLetter = line[lastEl];
    if (!opt->cOpt && !opt->lOpt) {
      if (!opt->hOpt) fprintf(stdout, "%s:", filename);
      if (opt->nOpt && !opt->lOpt) fprintf(stdout, "%d:", *lineNumber);
      if (!opt->oOpt) {
        fprintf(stdout, "%s", line);
        if (lastLetter != '\n') fprintf(stdout, "\n");
      }
      if (opt->oOpt && !opt->lOpt) {
        if (opt->vOpt) {
          fprintf(stdout, "%s", line);
          if (lastLetter != '\n') fprintf(stdout, "\n");
        } else {
          grepPrintO(preg, line, pmatch, filename, opt, *lineNumber);
        }
      }
    }
  }
}

void optInit(enabledOptions *opt) {
  opt->eOpt = 0;
  opt->iOpt = 0;
  opt->vOpt = 0;
  opt->cOpt = 0;
  opt->lOpt = 0;
  opt->nOpt = 0;
  opt->hOpt = 0;
  opt->sOpt = 0;
  opt->fOpt = 0;
  opt->oOpt = 0;
  opt->noOpt = 1;
  opt->result = 1;
}

int parseFlagsPatternsAndFiles(int argc, char ***argv, enabledOptions *opt,
                               char *patterns, int *argFileInd) {
  opterr = 0;
  while (optind < argc && opt->result) {
    getOptFunc(argc, argv, opt, patterns, argFileInd);
  }
  if (opt->eOpt == 0 && opt->fOpt == 0) {
    int index = 1;
    char *tmp = *(*argv + index);
    if (tmp[0] != '-')
      strcat(patterns, tmp);
    else {
      while (tmp[0] == '-') {
        index++;
        tmp = *(*argv + index);
      }
      strcat(patterns, tmp);
    }
    *argFileInd = *argFileInd + 1;
  }
  return 0;
}

void getOptFunc(int argc, char ***argv, enabledOptions *opt, char *patterns,
                int *argFileInd) {
  int res;
  FILE *fl = NULL;
  if ((res = getopt(argc, *argv, "e:ivclnhsf:o")) != -1) {
    switchingOptions(res, opt, patterns, &fl);
  } else {
    if (*argFileInd == -1) *argFileInd = optind;
    optind++;
  }
}

void switchingOptions(int res, enabledOptions *opt, char *patterns, FILE **fl) {
  switch (res) {
    case 'e':
      opt->eOpt = 1;
      addPattern(patterns, optarg);
      gotOption(opt);
      break;
    case 'i':
      opt->iOpt = 1;
      gotOption(opt);
      break;
    case 'v':
      opt->vOpt = 1;
      gotOption(opt);
      break;
    case 'c':
      opt->cOpt = 1;
      gotOption(opt);
      break;
    case 'l':
      opt->lOpt = 1;
      gotOption(opt);
      break;
    case 'n':
      opt->nOpt = 1;
      gotOption(opt);
      break;
    case 'h':
      opt->hOpt = 1;
      gotOption(opt);
      break;
    case 's':
      opt->sOpt = 1;
      gotOption(opt);
      break;
    case 'f':
      opt->fOpt = 1;
      if (checkFile(optarg, fl, 1))
        addPatternsFromFile(patterns, fl, opt);
      else
        opt->result = 0;
      gotOption(opt);
      break;
    case 'o':
      opt->oOpt = 1;
      gotOption(opt);
      break;
    case '?':
      errorOption(opt);
  }
}

void errorOption(enabledOptions *opt) {
  if (opt->sOpt == 0) {
    if (optopt != 'f' && optopt != 'e')
      illegalOptionErrorPrint(optopt);
    else
      noArgErrorPrint(optopt);
  }
  opt->result = 0;
}

void addPattern(char *patterns, char *pattern) {
  if (strlen(patterns) > 0) strcat(patterns, "|");
  strcat(patterns, pattern);
}

void addPatternsFromFile(char *patterns, FILE **fl, enabledOptions *opt) {
  opt->fOpt = 1;
  signed char tmp;
  int emptyLine = 0;
  if (strlen(patterns) != 0) {
    patterns += strlen(patterns);
    *patterns = '|';
    patterns++;
  }
  while ((tmp = fgetc(*fl)) != EOF) {
    if (tmp != '\n') {
      *patterns = tmp;
      if (emptyLine) emptyLine = 0;
    } else {
      if (!emptyLine)
        *patterns = '|';
      else {
        *patterns = '.';
        patterns++;
        *patterns = '|';
      }
      emptyLine = 1;
    }
    patterns++;
  }
  fclose(*fl);
}

void noArgErrorPrint(char ch) {
  fprintf(stderr, "grep: option requires an argument -- %c\n", ch);
  grepUsagePrint();
}

void illegalOptionErrorPrint(char ch) {
  fprintf(stderr, "grep: invalid option -- %c\n", ch);
  grepUsagePrint();
}

void grepUsagePrint() {
  fprintf(stderr,
          "usage: grep [-ivclnhso] [-e pattern] [-f file] [file ...]\n");
}

void gotOption(enabledOptions *opt) {
  if (opt->noOpt) {
    opt->noOpt = 0;
  }
}
