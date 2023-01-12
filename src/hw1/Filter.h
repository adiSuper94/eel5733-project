#pragma once
#include <stdbool.h>
#include <stdlib.h>

struct CalendarEvent{
    char *action, *title, *date, *time, *location;
   
};

struct CalendarEvent* getCalendarEvent(char *subject);
char* filterEmail(char* subject);

static inline void free_event(struct CalendarEvent *event){
    free(event->action);
    free(event->title);
    free(event->date);
    free(event->time);
    free(event->location);
    free(event);
}

static inline struct CalendarEvent* initCalendarEvent(char *action, char *title, char *date, char *time, char *location){
    struct CalendarEvent *event = (struct CalendarEvent*) malloc(sizeof(struct CalendarEvent));
    event->action = action;
    event->title = title;
    event->location = location;
    event->time = time;
    event->date = date;
    return event;
}
