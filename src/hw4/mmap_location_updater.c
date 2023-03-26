#include "../hw1/filter.h"
#include "./shared_ring_buffer.h"
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define IPC_SEM_MUT "/ipc_sem_mut"
#define IPC_SEM_FULL "/ipc_sem_full"
#define IPC_SEM_EMPTY "/ipc_sem_empty"
#define OUT_SEM_MUT "out_sem_mut"
#define OUT_SEM_FULL "out_sem_full"
#define OUT_SEM_EMPTY "out_sem_empty"

void init_semaphores(int capacity) {
  sem_unlink(IPC_SEM_EMPTY);
  sem_unlink(IPC_SEM_MUT);
  sem_unlink(IPC_SEM_FULL);
  sem_open(IPC_SEM_MUT, O_CREAT, 0777, 1);
  sem_open(IPC_SEM_FULL, O_CREAT, 0777, 0);
  sem_open(IPC_SEM_EMPTY, O_CREAT, 0777, capacity);
  // sem_open(OUT_SEM_MUT, O_CREAT, S_IROTH | S_IWOTH, 1);
  // sem_open(OUT_SEM_FULL, O_CREAT, S_IROTH | S_IWOTH, 0);
  // sem_open(OUT_SEM_EMPTY, O_CREAT, S_IROTH | S_IWOTH, CAPACITY);
}

void emailFilter(struct SharedRingBuffer *srb) {
  char *line = malloc(sizeof(char) * 70);
  while (scanf("%[^\n]\n", line) != EOF) {
    char *res = filterEmail(line);
    if (res != NULL) {
      insertIntoSharedRingBuf(srb, res);
    }
  }
  free(line);
  insertIntoSharedRingBuf(srb, "EOF");
}

void calendarFilter(struct SharedRingBuffer *srb) {
  char *line;
  struct Calendar *cal = createCalendar();
  while (true) {
    line = removeFromSharedRingBuff(srb);
    if (strcmp(line, "EOF") == 0) {
      break;
    }
    char *output = filterCalendarEvent(line, cal);
    free(line);
    if (output == NULL) {
      continue;
    }
    printf("%s\n", output);
    free(output);
  }
  freeCalendar(cal);
}

void createFilterProcesses(int capacity) {
  struct SharedRingBuffer *srb =
      initSharedRingBuffer(capacity, IPC_SEM_MUT, IPC_SEM_FULL, IPC_SEM_EMPTY);
  switch (fork()) {
  case -1:
    perror("Error forking for email filter");
    exit(EXIT_FAILURE);
    break;
  case 0:
    emailFilter(srb);
    break;
  default:
    calendarFilter(srb);
    break;
  }
  return;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Error: Call this bin with an integer arg, specifying the size of the buffer/queue!\n");
    exit(1);
  }
  int capacity = atoi(argv[1]);
  if (capacity <= 0) {
    printf("Error: Invalid args!! Call bin with buff/queue size > 0\n");
    exit(2);
  }
  init_semaphores(capacity);
  createFilterProcesses(capacity);
  exit(EXIT_SUCCESS);
}
