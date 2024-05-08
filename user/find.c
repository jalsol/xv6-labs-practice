#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

static void find(char* path, char* name);
static char* get_tail(char* path);
static void catdir(char* path, char* name, char* out);

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(2, "Usage: find [path] [name]\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}

void find(char* path, char* name) {
  int fd = open(path, 0);
  if (fd < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  struct stat st;
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  struct dirent de;
  switch (st.type) {
    case T_FILE:
      if (strcmp(get_tail(path), name) == 0) {
        printf("%s\n", path);
      }
      break;

    case T_DIR: {
      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) {
          continue;
        }
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
          continue;
        }

        char child[512];
        catdir(path, de.name, child);
        find(child, name);
      }
    } break;
  }

  close(fd);
}

void catdir(char* path, char* name, char* out) {
  /* printf("catting %s . %s\n", path, name); */
  uint path_len = strlen(path);
  uint name_len = strlen(name);

  memcpy(out, path, path_len);
  out[path_len] = '/';
  memcpy(out + path_len + 1, name, name_len);
  out[path_len + 1 + name_len] = '\0';
}

char* get_tail(char* path) {
  static char buf[DIRSIZ + 1];

  char *p;
  for (p = path + strlen(path); p >= path && *p != '/'; p--) {}
  p++;

  int len = strlen(p);

  if (len >= DIRSIZ) {
    return p;
  }

  memmove(buf, p, len);
  memset(buf + len, '\0', DIRSIZ - len);
  return buf;
}
