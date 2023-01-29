#pragma once
#include "calendar.h"
#include <stdbool.h>

struct CalendarEvent *parseEmailSubject(char *subject);
char *filterEmail(char *subject);
void freeMem(char *action, char *title, char *date, char *time, char *location);
bool isValidString(char *str, int size);
