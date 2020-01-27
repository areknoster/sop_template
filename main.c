#define _GNU_SOURCE
#include <aio.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define ERR(source)                                                            \
  (fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source),             \
   kill(0, SIGKILL), exit(EXIT_FAILURE))
#define KB_TO_BYTES(x) (x * 1024)

enum {
  ARGNO = 4,
};

typedef struct {
  char name[MAX_FNAME];
  int size;
} file_t;

typedef struct {
} state_t;

void usage(char *progname) {
  fprintf(stderr, "%s workfile blocksize\n", progname);
  fprintf(stderr, "workfile - path to the file to work on\n");
  fprintf(stderr, "n - number of blocks\n");
  fprintf(stderr, "k - number of iterations\n");
  exit(EXIT_FAILURE);
}

int strToNum(char *str) {
  errno = 0;
  int v = strtol(str, NULL, 10);
  if (errno != 0)
    perror("wrong int value!");
  return v;
}

int getFileSize(char *fname) {
  struct stat buf;
  if (stat(fname, &buf))
    ERR("file stat");
  return buf.st_size;
}

void sethandler(void (*f)(int), int sig) {
  struct sigaction sa;
  memset(&sa, 0x00, sizeof(struct sigaction));
  sa.sa_handler = f;
  if (sigaction(sig, &sa, NULL) == -1)
    ERR("Error setting signal handler");
}

void readArgs(state_t *state, int argc, char **argv) {
  if (argc != ARGNO)
    usage(argv[0]);
}

void readFilenames(state_t *state) {
  DIR *dir;
  struct dirent *ent;
  int num = 0;
  if ((dir = opendir(state->dirName)) != NULL) {
    if (chdir(state->dirName))
      ERR("couldnt change current directory");
    while (NULL != (ent = readdir(dir))) {
      if (ent->d_type != DT_REG)
        continue;
      if (getFileSize(ent->d_name) < state->blockSize)
        num++;
    }
    if (NULL == (state->files = calloc(num, sizeof(file_t))))
      ERR("calloc");
    state->fileNo = num;
    rewinddir(dir);
    num = 0;
    while ((ent = readdir(dir)) != NULL) {
      if (ent->d_type != DT_REG)
        continue;
      if (getFileSize(ent->d_name) < state->blockSize) {
        strcpy(state->files[num].fileName, ent->d_name);
        state->files[num].fileSize = getFileSize(ent->d_name);
        num++;
      }
    }
    closedir(dir);
  } else {
    perror("Could not open directory");
  }
}

void suspend(struct aiocb *aiocbs) {
  struct aiocb *aiolist[1];
  aiolist[0] = aiocbs;
  while (aio_suspend((const struct aiocb *const *)aiolist, 1, NULL) == -1) {
    if (errno == EINTR)
      continue;
    ERRNO("Suspend error");
  }
  if (aio_error(aiocbs) != 0)
    ERR("Suspend error");
  if (aio_return(aiocbs) == -1)
    ERR("Return error");
}

void syncdata(struct aiocb *aiocbs) {
  if (!work)
    return;
  suspend(aiocbs);
  if (aio_fsync(O_SYNC, aiocbs) == -1)
    ERR("Cannot sync\n");
  suspend(aiocbs);
}

void cleanup(state_t *state) {}

int main(int argc, char **argv) {
  state_t state = (const state_t){0};
  readArgs(&state, argc, argv);

  cleanup(&state);
  return EXIT_SUCCESS;
}