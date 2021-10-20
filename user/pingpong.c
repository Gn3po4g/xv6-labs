#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int parentpipe[2], childpipe[2];
  char buf[5];
  pipe(parentpipe);
  pipe(childpipe);
  if (fork() == 0) {
    read(parentpipe[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);
    write(childpipe[1], "pong", strlen("pong"));
  } else {
    write(parentpipe[1], "ping", strlen("ping"));
    wait(0);
    read(childpipe[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);
  }
  exit(0);
}