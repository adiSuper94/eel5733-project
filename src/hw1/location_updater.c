#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void createEmailFilterProcess(int ipcPipe[], int inputPipe[]);
void createCalendarFilterProcess(int ipcPipe[], int inputPipe[]);

int main() {
  /*
   * main process reads input from STDIN and writes into inputPipe[1]
   * email filter child process reads from inputPipe[0] and writes into ipcPipe[1]
   * calendar filter child process read from ipcPipe[0] and write into STDOUT
   */
  int ipcPipe[2], inputPipe[2];
  if (pipe(ipcPipe) == -1 || pipe(inputPipe) == -1) {
    perror("Error while creating pipe.");
    exit(EXIT_FAILURE);
  }
  createEmailFilterProcess(ipcPipe, inputPipe);
  createCalendarFilterProcess(ipcPipe, inputPipe);

  char *line = malloc(sizeof(char) * 100);
  while (scanf("%[^\n]\n", line) != EOF) {
    strcat(line, "\n");
    write(inputPipe[1], line, strlen(line));
    // printf("input: %s", line);
  }
  free(line);
  close(ipcPipe[0]);
  close(inputPipe[1]); // closing input write end of input pipe. this emits EOF
  close(ipcPipe[1]);
  close(inputPipe[0]);
  wait(NULL);
  wait(NULL);
  exit(EXIT_SUCCESS);
}

void createEmailFilterProcess(int ipcPipe[], int inputPipe[]) {
  switch (fork()) {
  case -1:
    perror("Error forking for email filter");
    exit(EXIT_FAILURE);
    break;
  case 0:
    // Child process
    close(inputPipe[1]); // closing unused write end of input pipe.
    close(ipcPipe[0]);   // closing unused read end of ipc pipe.
    // FDs remain valid even accross exec calls
    dup2(inputPipe[0], STDIN_FILENO);
    dup2(ipcPipe[1], STDOUT_FILENO);
    //  This close only reduces the reference counter in system file descriptor table
    //  close pipes after use
    close(inputPipe[0]);
    close(ipcPipe[1]);
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, "/email_filter");
    execl(cwd, "email_filter", (char *)NULL);
    exit(EXIT_FAILURE);
    break;
  default:
    return;
  }
  return;
}

void createCalendarFilterProcess(int ipcPipe[], int unusedInputPipe[]) {
  switch (fork()) {
  case -1:
    perror("Error forking for calendar filter");
    exit(EXIT_FAILURE);
    break;
  case 0:
    // Child process
    close(ipcPipe[1]); // closing unused write end of ipc pipe.
    // inputPipe not used by calendar filter
    close(unusedInputPipe[0]);
    close(unusedInputPipe[1]);
    // FDs remain valid even accross exec calls
    dup2(ipcPipe[0], STDIN_FILENO);
    //  This close only reduces the reference counter in system file descriptor table
    close(ipcPipe[0]);
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    strcat(cwd, "/calendar_filter");
    execl(cwd, "calendar_filter", (char *)NULL);
    exit(EXIT_FAILURE);
    break;
  default:
    return;
  }
  return;
}
