#include "./queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

struct Entry {
  char *c;
  TAILQ_ENTRY(Entry) entries;
};

TAILQ_HEAD(tailhead, Entry) head;

void initQ() { TAILQ_INIT(&head); }

void insertIntoQ(char *line) {
  struct Entry *entry = malloc(sizeof(struct Entry));
  entry->c = line;
  TAILQ_INSERT_HEAD(&head, entry, entries);
}

char *removeFromQ() {
  if (TAILQ_EMPTY(&head)) {
    return NULL;
  }

  struct Entry *entry = TAILQ_LAST(&head, tailhead);
  TAILQ_REMOVE(&head, entry, entries);
  char *line = entry->c;
  free(entry);
  return line;
}

int sizeOfQ() {
  int size = 0;
  struct Entry *entry;
  TAILQ_FOREACH(entry, &head, entries) { size++; }
  return size;
}
