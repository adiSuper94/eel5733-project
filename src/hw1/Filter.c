#include "Filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void freeMem(char *action, char *title, char *date, char *time, char *location){
  free(action);
  free(title);
  free(date);
  free(time);
  free(location);
  return;
}

bool isValidString(char *str, int size){
    int k = size > strlen(str) ? strlen(str) : size;
    for(int i = 0; i < k ; i++){
        if(!isprint(*(str + (i * sizeof(char))))){
            return false;
        }
    }
    return true;
}

char* filterEmail(char *line){
  char *subject = malloc(sizeof(char) * 100);
  sscanf(line, "Subject: %[^\n]", subject);
  struct CalendarEvent *event = getCalendarEvent(subject);
  if(event != NULL){
    free_event(event);
    return subject;
  }
  free(subject);
  return NULL;
}

struct CalendarEvent* getCalendarEvent(char *subject){
  char *action = malloc(sizeof(char) * 2);
  char *title = malloc(sizeof(char) * 20);
  char *location = malloc(sizeof(char) * 20);
  char *time = malloc(sizeof(char) * 20);
  char *date = malloc(sizeof(char) * 20);
  sscanf(subject, "%[^,],%10[^,],%[^,a-zA-Z],%[^,a-zA-Z],%s", action, title, date, time, location);

  if(!isValidString(title, 20) || !isValidString(date, 20) || !isValidString(time, 20) || !isValidString(location, 20)){
    freeMem(action, title, date, time, location);
    return NULL;
  }
  if(strcmp(action, "C") !=0 && strcmp(action, "D") !=0 && strcmp(action, "X") !=0){
    freeMem(action, title, date, time, location);
    return NULL;
  }
  struct CalendarEvent *event = initCalendarEvent(action, title, date, time, location);
  return event;
}


