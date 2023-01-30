#pragma once
#include <stdlib.h>

struct CalendarEvent {
  char *action, *title, *date, *time, *location;
  struct CalendarEvent *next;
};

struct CalendarDate {
  char *date;
  struct CalendarEvent *events;
  struct CalendarDate *nextDate;
  struct CalendarEvent *earliestEvent;
};

struct Calendar {
  struct CalendarDate *calendarDate;
};

int insertCalendarEvent(struct Calendar *cal, char *date, struct CalendarEvent *calendarEvent);
struct CalendarDate *getCalendarDate(struct Calendar *calendar, char *date);
struct CalendarEvent *getCalendarEvent(struct Calendar *cal, char *date, char *title);
int updateCalendarEvent(struct Calendar *cal, char *date, struct CalendarEvent *calendarEvent);
struct CalendarEvent *getEarliestEvent(struct Calendar *cal, char *date);
struct CalendarDate *getCalendarDate(struct Calendar *cal, char *date);

static inline struct CalendarEvent *createCalendarEvent(char *action, char *title, char *date,
                                                        char *time, char *location) {
  struct CalendarEvent *event = (struct CalendarEvent *)malloc(sizeof(struct CalendarEvent));
  event->action = action;
  event->title = title;
  event->location = location;
  event->time = time;
  event->date = date;
  return event;
}
static inline struct Calendar *createCalendar() {
  struct Calendar *cal = (struct Calendar *)malloc(sizeof(struct Calendar));
  cal->calendarDate = NULL;
  return cal;
}
static inline void free_event(struct CalendarEvent *event) {
  free(event->action);
  free(event->title);
  free(event->date);
  free(event->time);
  free(event->location);
  free(event);
}

static inline void freeDate(struct CalendarDate *date) {
  struct CalendarEvent *currEvent = date->events;
  while (currEvent != NULL) {
    struct CalendarEvent *nextEvent = currEvent->next;
    free_event(currEvent);
    currEvent = nextEvent;
  }
}
static inline void freeCalendar(struct Calendar *cal) {
  struct CalendarDate *currDate = cal->calendarDate;
  while (currDate != NULL) {
    struct CalendarDate *nextDate = currDate->nextDate;
    freeDate(currDate);
    free(currDate->date);
    free(currDate);
    currDate = nextDate;
  }
  free(cal);
}
