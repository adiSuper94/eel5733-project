#pragma once
#include <semaphore.h>
#include <stdlib.h>

struct RingBuffer {
  unsigned int capacity;
  int head;
  int tail;
  sem_t mut;
  sem_t full, empty;
  char **buffer;
};

struct RingBuffer *initRingBuffer(unsigned int capacity);
void insertIntoRingBuf(struct RingBuffer *rb, char *line);
char *removeFromRingBuff(struct RingBuffer *rb);
