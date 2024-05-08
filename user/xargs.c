#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		fprintf(2, "Usage: xargs <command> [argv...]\n");
		exit(1);
	}

  int n_args;
	char* args[MAXARG];
	for (n_args = 1; n_args < argc; n_args++) {
		args[n_args - 1] = argv[n_args];
	}
	--n_args;

	int blanks = 0;
	int offset = 0;
	char buf[512], ch;
	char* p = buf;

	while (read(0, &ch, 1) > 0) {
		if (ch == ' ') {
			blanks = 1;
			continue;
		}

		if (blanks) {
			buf[offset++] = 0;

			args[n_args++] = p;
			p = buf + offset;

			blanks = 0;
		}

		if (ch != '\n') {
			buf[offset++] = ch;
		} else {
			args[n_args++] = p;
			p = buf + offset;

			if (!fork()) {
				exit(exec(args[0], args));
			}
			wait(0);
			
			n_args = argc - 1;
		}
	}

	exit(0);
}
