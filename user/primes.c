#include "kernel/types.h"
#include "user/user.h"

enum { N = 35 };

static void filter(int prev_fd);

int main(void) {
  int pid[2];
  pipe(pid);

  // generate
  for (int i = 2; i <= N; ++i) {
    write(pid[1], &i, sizeof(i));
  }
  close(pid[1]);

  filter(pid[0]);
  close(pid[0]);

  exit(0);
}

void filter(int prev_fd) {
  int pid[2];
  pipe(pid);

  int head;
  if (read(prev_fd, &head, sizeof(head)) != sizeof(head)) {
    close(pid[0]);
    close(pid[1]);
    return;
  }

  printf("prime %d\n", head);

  int num = -1;
  while (read(prev_fd, &num, sizeof(num)) == sizeof(num)) {
    if (num % head != 0) {
      write(pid[1], &num, sizeof(num));
    }
  }

  if (num == -1) {
    close(pid[0]);
    close(pid[1]);
    return;
  }

  if (fork()) {
    close(pid[0]);
    close(pid[1]);
    wait(0);
  } else {
    close(pid[1]);
    filter(pid[0]);
    close(pid[0]);
  }
}
