#include "kernel/types.h"
#include "user/user.h"

int main(const int argc, const char* argv[]) {
  if (argc == 1) {
    printf("sleep: No argument.\n");
    exit(1);
  }

  const int duration = 10 * atoi(argv[1]);
  sleep(duration);

  exit(0);
}
