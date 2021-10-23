#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int found = 0;

void find(char *path, char *filename) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type != T_DIR) {
    fprintf(2, "find: %s is not a directory\n", path);
    close(fd);
    return;
  }
  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
    fprintf(2, "find: directory too long\n");
    close(fd);
    return;
  }
  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0)
      continue;
    if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if (stat(buf, &st) < 0) {
      printf("find: cannot stat %s\n", buf);
      continue;
    }
    if (st.type == T_DIR)
      find(buf, filename);
    else if (st.type == T_FILE && !strcmp(de.name, filename)) {
      printf("%s\n", buf);
      found = 1;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage: find <path> <file>\n");
    exit(1);
  } else
    find(argv[1], argv[2]);
  if (!found)
    printf("cannot find %s\n", argv[2]);
  exit(0);
}