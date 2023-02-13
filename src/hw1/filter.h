#pragma once
#include "calendar.h"
#include <stdbool.h>

struct CalendarEvent *parseEmailSubject(char *subject);
char *filterEmail(char *subject);
char *filterCalendarEvent(char *eventStr, struct Calendar *cal);
void freeMem(char *action, char *title, char *date, char *time, char *location);
bool isValidString(char *str, int size);
