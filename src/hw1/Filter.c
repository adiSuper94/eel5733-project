#include "Filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* filterEmail(char *line){
  char *subject = malloc(sizeof(char) * 100);
  sscanf(line, "Subject: %[^\n]", subject);
  if(getCalendarEvent(subject) != NULL){
    return subject;
  }
  return NULL;
}
   
struct CalendarEvent* getCalendarEvent(char *subject){
  char *action = malloc(sizeof(char) * 2);
  char *title = malloc(sizeof(char) * 20);
  char *location = malloc(sizeof(char) * 20);
  char *time = malloc(sizeof(char) * 20);
  char *date = malloc(sizeof(char) * 20);
  sscanf(subject, "%[^,],%[^,],%[^,],%[^,],%s", action, title, date, time, location);

  if(strcmp(title, "") == 0|| strcmp(date, "") == 0 || strcmp(time, "") == 0 || strcmp(location, "") == 0){
    return NULL;
  }
  if(strcmp(action, "C") !=0 && strcmp(action, "D") !=0 && strcmp(action, "X") !=0){
    return NULL;
  }
  struct CalendarEvent *event = (struct CalendarEvent*) malloc(sizeof(struct CalendarEvent));
  event->action = action;
  event->title = title;
  event->location = location;
  event->time = time;
  event->date = date;
  return event;
}


