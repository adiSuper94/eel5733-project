#pragma once
#include <semaphore.h>
#include <stdlib.h>

#define STRING_LEN 100

struct SharedRingBuffer {
  int capacity;
  int head;
  int tail;
  sem_t *mut;
  sem_t *full, *empty;
  char (*buffer)[STRING_LEN];
};

struct SharedRingBuffer *initSharedRingBuffer(int capacity, const char *sem_mut_name,
                                              const char *sem_full_name,
                                              const char *sem_empty_name);

void insertIntoSharedRingBuf(struct SharedRingBuffer *rb, char *line);
char *removeFromSharedRingBuff(struct SharedRingBuffer *rb);
