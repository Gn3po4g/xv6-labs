#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: sleep <ticks>\n");
    exit(1);
  } else {
    sleep(atoi(argv[1]));
    printf("(nothing happens for a little while)\n");
  }
  exit(0);
}