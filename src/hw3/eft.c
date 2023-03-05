#include "bank.h"
#include "ringbuffer.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  FILE *inputFile;
  char *line;
  size_t lineLen = 0;
  if (argc != 3) {
    printf("Error: Call this bin with a file path and an integer arg, specifying the number of "
           "worker threads\n");
    exit(1);
  }
  inputFile = fopen(argv[1], "r");
  if (inputFile == NULL) {
    printf("Error: could not open file");
    exit(1);
  }
  int workerCount = atoi(argv[2]);
  if (workerCount <= 0) {
    printf("Error: Invalid args!! Call bin with worker count > 0\n");
    exit(1);
  }

  struct RingBuffer **buffers = malloc(sizeof(struct RingBuffer *) * workerCount);
  pthread_t workers[workerCount];
  struct Bank *bank = initBank();
  for (int i = 0; i < workerCount; i++) {
    buffers[i] = initRingBuffer(2);
    struct TellerThreadArg *tellerThreadArg = malloc(sizeof(struct TellerThreadArg));
    tellerThreadArg->bank = bank;
    tellerThreadArg->rb = buffers[i];
    pthread_create(&workers[i], NULL, (void *)*tellerInit, (void *)tellerThreadArg);
  }
  int ctr = 0;
  while (getline(&line, &lineLen, inputFile) != EOF) {
    if (line[0] != 'T') {
      int accountNumber, initBalance;
      sscanf(line, "%d %d", &accountNumber, &initBalance);
      addAccount(bank, accountNumber, initBalance);
      continue;
    }
    insertIntoRingBuf(buffers[ctr % workerCount], line);
    ctr++;
  }
  for (int i = 0; i < workerCount; i++) {
    insertIntoRingBuf(buffers[i], "END");
  }
  for (int i = 0; i < workerCount; i++) {
    pthread_join(workers[i], NULL);
  }
  printResult(bank);
  fclose(inputFile);
}
