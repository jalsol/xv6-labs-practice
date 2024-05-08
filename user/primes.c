#include "kernel/types.h"
#include "user/user.h"

enum { PARENT, CHILD };
enum { N = 35 };

int main(void) {
  int pid[2];

  int n = 0;
  int nums[N];
  for (int i = 2; i <= N; ++i) {
    nums[n++] = i;
  }

  int buf[N];
  int n_buf = 0;

  while (n > 0) {
    pipe(pid);
    int fpid = fork();

    if (fpid != 0) {
      write(pid[CHILD], nums, n * sizeof(int));
      wait((int*) 0);
      close(pid[PARENT]);
      close(pid[CHILD]);
      break;
    }

    if (fpid == 0) {
      int bytes_read = read(pid[PARENT], buf, 4 * N);
      n_buf = bytes_read / sizeof(int);
      close(pid[PARENT]);
      close(pid[CHILD]);

      if (n_buf == 0) {
        exit(1);
      }

      int head = buf[0];
      n = 0;
      printf("prime %d\n", head);
      for (int i = 0; i < n_buf; ++i) {
        if (buf[i] % head != 0) {
          nums[n++] = buf[i];
        }
      }
    }
  }

  exit(0);
}
