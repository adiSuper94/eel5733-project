#include "../hw1/filter.h"
#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define Q_SIZE 1

pthread_mutex_t q_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t q_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t q_not_empty = PTHREAD_COND_INITIALIZER;

void emailFilterThread();
void calendarFilterThread();

char *safeRead();
void safeWrite(char *line);

int main() {
  pthread_t emailFilter_t, calendarFilter_t;
  void *etStatus, *ctStatus;
  initQ();
  pthread_create(&emailFilter_t, NULL, (void *)*emailFilterThread, NULL);
  pthread_create(&calendarFilter_t, NULL, (void *)*calendarFilterThread, NULL);
  pthread_join(emailFilter_t, &etStatus);
  pthread_join(calendarFilter_t, &ctStatus);
  pthread_exit(NULL);
}

void emailFilterThread() {
  char *line = malloc(sizeof(char) * 100);
  while (scanf("%[^\n]\n", line) != EOF) {
    char *res = filterEmail(line);
    if (res != NULL) {
      safeWrite(res);
    }
  }
  free(line);
  safeWrite("EOF");
  pthread_exit(0);
}

void calendarFilterThread() {
  char *line;
  struct Calendar *cal = createCalendar();
  while (true) {
    line = safeRead();
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
  pthread_exit(0);
}

char *safeRead() {
  char *line;
  pthread_mutex_lock(&q_mutex);
  if (sizeOfQ() <= 0) {
    pthread_cond_wait(&q_not_empty, &q_mutex);
  }
  line = removeFromQ();
  pthread_cond_signal(&q_not_full);
  pthread_mutex_unlock(&q_mutex);
  return line;
}

void safeWrite(char *line) {
  pthread_mutex_lock(&q_mutex);
  if (sizeOfQ() >= Q_SIZE) {
    pthread_cond_wait(&q_not_full, &q_mutex);
  }
  insertIntoQ(line);
  pthread_cond_signal(&q_not_empty);
  pthread_mutex_unlock(&q_mutex);
  return;
}
