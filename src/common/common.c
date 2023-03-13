#include "common.h"

int checkFile(const char* path, FILE** fl, int decision) {
  int result = 1;
  DIR* dir = opendir(path);
  if (dir) {
    if (decision) noFileErrorPrint(path, 1);
    result = 0;
    closedir(dir);
  } else {
    *fl = fopen(path, "r");
    if (*fl == NULL) {
      if (decision) noFileErrorPrint(path, 0);
      result = 0;
    }
  }
  return result;
}

int noFileErrorPrint(const char* fileName, int isDir) {
#ifdef CAT_USAGE
  fprintf(stderr, "cat: ");
#endif
#ifdef GREP_USAGE
  fprintf(stderr, "grep: ");
#endif
  if (isDir) {
    fprintf(stderr, "%s: Is a directory\n", fileName);
  } else {
    fprintf(stderr, "%s: No such file or directory\n", fileName);
  }
  return 0;
}
