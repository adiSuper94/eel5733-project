#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void createEmailFilterProcess(int ipcPipe[], int inputPipe[], int outputPipe[]);
void createCalendarFilterProcess(int ipcPipe[], int outputPipe[], int inputPipe[]);

int main() {
  /*
   * main process reads input from STDIN and writes into inputPipe[1]
   * email filter child process reads from inputPipe[0] and writes into ipcPipe[1]
   * calendar filter child process read from ipcPipe[0] and write into outputPipe[1]
   *  main process reads from outputPipe[0] and writes into STDOUT
   */
  int ipcPipe[2], inputPipe[2], outputPipe[2];
  if (pipe(ipcPipe) == -1 || pipe(inputPipe) == -1) {
    perror("Error while creating pipe.");
    exit(EXIT_FAILURE);
  }
  createEmailFilterProcess(ipcPipe, inputPipe, outputPipe);
  createCalendarFilterProcess(ipcPipe, outputPipe, inputPipe);

  // closing unused pipe ends in main/parent process
  close(ipcPipe[0]);
  close(ipcPipe[1]);
  close(inputPipe[0]);
  close(outputPipe[1]);

  char *line = malloc(sizeof(char) * 100);
  while (scanf("%[^\n]\n", line) != EOF) {
    write(inputPipe[1], line, strlen(line));
  }
  close(inputPipe[1]); // closing input write end of input pipe. this emits EOF
  printf("TODO:: Read from outputPipe");
  close(outputPipe[0]); // closing output read pipe
  exit(EXIT_SUCCESS);
}

void createEmailFilterProcess(int ipcPipe[], int inputPipe[], int unusedOutputPipe[]) {
  switch (fork()) {
  case -1:
    perror("Error forking for email filter");
    exit(EXIT_FAILURE);
    break;
  case 0:
    // Child process
    close(inputPipe[1]); // closing unused write end of input pipe.
    close(ipcPipe[0]);   // closing unused read end of ipc pipe.
    // output pipe not used by email filter child process
    close(unusedOutputPipe[0]);
    close(unusedOutputPipe[1]);
    printf("TODO: flavour of exec for email-filter");
    // close pipes after use
    close(inputPipe[0]);
    close(ipcPipe[1]);
    exit(EXIT_FAILURE);
    break;
  default:
    return;
  }
  return;
}

void createCalendarFilterProcess(int ipcPipe[], int outputPipe[], int unusedInputPipe[]) {
  switch (fork()) {
  case -1:
    perror("Error forking for calendar filter");
    exit(EXIT_FAILURE);
    break;
  case 0:
    // Child process
    close(outputPipe[0]); // closing unused read end of output pipe.
    close(ipcPipe[1]);    // closing unused write end of ipc pipe.
    // inputPipe not used by calendar filter
    close(unusedInputPipe[0]);
    close(unusedInputPipe[1]);
    printf("TODO: flavour of exec for email-filter");
    // close pipes after use
    close(outputPipe[1]);
    close(ipcPipe[0]);
    exit(EXIT_FAILURE);
    break;
  default:
    return;
  }
  return;
}
