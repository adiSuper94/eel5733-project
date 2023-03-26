#include "./shared_ring_buffer.h"
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

struct SharedRingBuffer *initSharedRingBuffer(int capacity, const char *sem_mut_name,
                                              const char *sem_full_name,
                                              const char *sem_empty_name) {
  struct SharedRingBuffer *rb = mmap(NULL, sizeof(struct SharedRingBuffer), PROT_READ | PROT_WRITE,
                                     MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  rb->capacity = capacity;
  rb->head = 0;
  rb->tail = 0;
  rb->mut = sem_open(sem_mut_name, 0);
  rb->full = sem_open(sem_full_name, 0);
  rb->empty = sem_open(sem_empty_name, 0);
  rb->buffer = mmap(NULL, (sizeof(char) * STRING_LEN * capacity + 1), PROT_READ | PROT_WRITE,
                    MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  if (rb->mut == NULL || rb->full == NULL || rb->empty == NULL || rb->buffer == NULL) {
    perror("Error while calling mmap");
    exit(EXIT_FAILURE);
  }
  return rb;
}

void insertIntoSharedRingBuf(struct SharedRingBuffer *rb, char *line) {

  sem_wait((rb->empty));
  sem_wait((rb->mut));
  if (strlen(line) >= STRING_LEN) {
    exit(1);
  }
  strcpy(rb->buffer[rb->tail++ % rb->capacity], line);
  sem_post((rb->full));
  sem_post((rb->mut));
}

char *removeFromSharedRingBuff(struct SharedRingBuffer *rb) {
  sem_wait((rb->full));
  sem_wait((rb->mut));
  char *poppedLine = rb->buffer[(rb->head++) % rb->capacity];
  char *lineCopy = malloc(sizeof(char) * (strlen(poppedLine) + 2));
  strcpy(lineCopy, poppedLine);

  sem_post((rb->empty));
  sem_post((rb->mut));
  return lineCopy;
}
