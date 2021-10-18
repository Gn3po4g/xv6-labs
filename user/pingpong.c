#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int parentpipe[2], childpipe[2];
  char buf[1];
  pipe(parentpipe);
  pipe(childpipe);
  if (fork() == 0) {
    close(parentpipe[0]);
    close(childpipe[1]);
    if (read(childpipe[0], buf, sizeof(buf)) != sizeof(buf)) {
      printf("child read error");
      exit(1);
    }
    close(childpipe[0]);
    printf("%d: received ping\n", getpid());
    if (write(parentpipe[1], buf, sizeof(buf)) != sizeof(buf)) {
      printf("child write error");
      exit(1);
    }
    close(parentpipe[1]);
    exit(0);
  } else {
    close(parentpipe[1]);
    close(childpipe[0]);
    if (write(childpipe[1], buf, sizeof(buf)) != sizeof(buf)) {
      printf("parent write error");
      exit(1);
    }
    close(childpipe[1]);
    if (read(parentpipe[0], buf, sizeof(buf)) != sizeof(buf)) {
      printf("parent read error");
      exit(1);
    }
    close(parentpipe[0]);
    printf("%d: received pong\n", getpid());
    wait(0);
    exit(0);
  }
}