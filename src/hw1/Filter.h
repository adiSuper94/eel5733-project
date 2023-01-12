#pragma once
#include <stdbool.h>

struct CalendarEvent{
    const char *action, *title, *date, *time, *location;
   
};

struct CalendarEvent* getCalendarEvent(char *subject);
char* filterEmail(char* subject);

