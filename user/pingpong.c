#include "kernel/types.h"
#include "user/user.h"

enum { PARENT, CHILD };

static int pid[2];

static inline void process_child(void);
static inline void process_parent(void);

int main(void) {
  pipe(pid);

  if (fork() == 0) {
    process_child();
  } else {
    process_parent();
  }

  exit(0);
}

void process_child(void) {
  printf("%d: received ping\n", pid[CHILD]);
  write(pid[PARENT], "b", 1);
  close(pid[PARENT]);
  close(pid[CHILD]);
}

void process_parent(void) {
  static char buf[1];

  write(pid[CHILD], "a", 1);
  wait((int*)0);
  read(pid[CHILD], buf, 1);
  printf("%d: received pong\n", pid[PARENT]);
  close(pid[PARENT]);
  close(pid[CHILD]);
}
