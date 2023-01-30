#include "calendar.h"
#include "filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * Need a map with key as date and and value as a list of calendar events
 */
int main() {
  char *line = malloc(sizeof(char) * 100);
  struct Calendar *cal = createCalendar();
  while (scanf("%[^\n]\n", line) != EOF) {
    struct CalendarEvent *event = parseEmailSubject(line);
    free(line);
    line = malloc(sizeof(char) * 100);
    if (event != NULL) {
      insertCalendarEvent(cal, event->date, event);
      struct CalendarEvent *earliestEvent = getEarliestEvent(cal, event->date);
      if (earliestEvent == NULL) {
        printf("%s,--:--,NA\n", event->date);
        continue;
      }
      struct CalendarDate *calDate = getCalendarDate(cal, event->date);
      if (strcmp(earliestEvent->title, event->title) != 0) {
        if (calDate->earliestEvent == NULL ||
            strcmp(calDate->earliestEvent->title, earliestEvent->title) == 0) {
          continue;
        }
      }
      calDate->earliestEvent = earliestEvent;
      printf("%s,%s,%s\n", earliestEvent->date, earliestEvent->time, earliestEvent->location);
      if (strcmp(event->action, "C") != 0) {
        free_event(event);
      }
    }
  }
  freeCalendar(cal);
  free(line);
  return 0;
}
