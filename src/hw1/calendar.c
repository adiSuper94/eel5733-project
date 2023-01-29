#include "calendar.h"
#include <stdio.h>
#include <string.h>

int updateCalendarEvent(struct Calendar *cal, char *date, struct CalendarEvent *calendarEvent) {
  if (strcmp(calendarEvent->action, "D") != 0 || strcmp(calendarEvent->action, "X") != 0) {
    return -2;
  }
  struct CalendarEvent *oldCalendarEvent = getCalendarEvent(cal, date, calendarEvent->title);
  if (oldCalendarEvent == NULL) {
    return -1;
  }
  strcpy(oldCalendarEvent->location, calendarEvent->location);
  strcpy(oldCalendarEvent->time, calendarEvent->time);
  strcpy(oldCalendarEvent->action, calendarEvent->action);

  free_event(calendarEvent);
  return 0;
}

int insertCalendarEvent(struct Calendar *cal, char *date, struct CalendarEvent *calendarEvent) {
  if (strcmp(calendarEvent->action, "D") == 0 || strcmp(calendarEvent->action, "X") == 0) {
    return updateCalendarEvent(cal, date, calendarEvent);
  }

  struct CalendarDate *calendarDate = getCalendarDate(cal, date);
  if (calendarDate == NULL) {
    char dateStr[20];
    calendarDate = (struct CalendarDate *)malloc(sizeof(struct CalendarDate));
    strcpy(dateStr, date);
    calendarDate->date = dateStr;
    calendarDate->events = NULL;
    calendarDate->nextDate = NULL;
  }
  struct CalendarEvent *lastEvent = calendarDate->events;
  calendarEvent->next = NULL;
  if (lastEvent == NULL) {
    calendarDate->events = calendarEvent;
    return 1;
  }
  while (lastEvent->next != NULL) {
    lastEvent = lastEvent->next;
  }
  lastEvent->next = calendarEvent;
  return 0;
}

struct CalendarEvent *getCalendarEvent(struct Calendar *cal, char *date, char *title) {
  struct CalendarDate *calendarDate = getCalendarDate(cal, date);
  struct CalendarEvent *event = calendarDate->events;
  while (event != NULL) {
    if (strcmp(title, event->title) == 0) {
      return event;
    }
  }
  return NULL;
}

struct CalendarDate *getCalendarDate(struct Calendar *cal, char *date) {
  struct CalendarDate *currCalDate = cal->calendarDate;
  while (currCalDate != NULL) {
    if (strcmp(currCalDate->date, date) == 0) {
      return currCalDate;
    }
    currCalDate = currCalDate->nextDate;
  }
  return NULL;
}

/*
 * returns -1 if time1 is greater
 * returns  1 if time2 is greater
 * returns  0 if both as same
 */
int timeCmp(char *time1, char *time2) {
  int h1, h2, m1, m2;
  sscanf(time1, "%d:%d", &h1, &m1);
  sscanf(time2, "%d:%d", &h2, &m2);
  if (h1 > h2) {
    return -1;
  } else if (h2 > h1) {
    return 1;
  } else {
    if (m1 > m2) {
      return -1;
    } else if (m2 > m1) {
      return 1;
    } else {
      return 0;
    }
  }
}

struct CalendarEvent *getEarliestEvent(struct Calendar *cal, char *date) {
  struct CalendarDate *calDate = getCalendarDate(cal, date);
  struct CalendarEvent *currEvent = calDate->events;
  struct CalendarEvent *earliestEvent = currEvent;
  while (currEvent != NULL) {
    if (timeCmp(currEvent->time, earliestEvent->time) == 1) {
      earliestEvent = currEvent;
    }
    currEvent = currEvent->next;
  }
  return earliestEvent;
}
