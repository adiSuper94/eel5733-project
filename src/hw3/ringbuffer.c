#include "./ringbuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct RingBuffer *initRingBuffer(unsigned int capacity) {
  struct RingBuffer *rb = malloc(sizeof(struct RingBuffer));
  rb->capacity = capacity;
  rb->buffer = malloc(sizeof(char *) * capacity);
  rb->head = 0;
  rb->tail = 0;
  if (sem_init(&(rb->mut), 0, 1) == -1) {
    exit(-1);
  }
  if (sem_init(&(rb->empty), 0, capacity) == -1) {
    exit(-1);
  }
  if (sem_init(&(rb->full), 0, 0) == -1) {
    exit(-1);
  }

  return rb;
}

void insertIntoRingBuf(struct RingBuffer *rb, char *line) {

  sem_wait(&(rb->empty));
  sem_wait(&(rb->mut));
  char *lineCopy = malloc(sizeof(char) * (strlen(line) + 2));
  strcpy(lineCopy, line);
  rb->buffer[rb->tail++ % rb->capacity] = lineCopy;
  sem_post(&(rb->full));
  sem_post(&(rb->mut));
}

char *removeFromRingBuff(struct RingBuffer *rb) {
  sem_wait(&(rb->full));
  sem_wait(&(rb->mut));
  char *poppedLine = rb->buffer[rb->head++ % rb->capacity];
  char *lineCopy = malloc(sizeof(char) * (strlen(poppedLine) + 2));
  strcpy(lineCopy, poppedLine);
  sem_post(&(rb->empty));
  sem_post(&(rb->mut));
  free(poppedLine);
  return lineCopy;
}
