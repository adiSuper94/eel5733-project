#include "calendar.h"
#include "filter.h"
#include <stdio.h>
#include <stdlib.h>
/**
 * Need a map with key as date and and value as a list of calendar events
 */
int main() {
  char *line = malloc(sizeof(char) * 100);
  struct Calendar *cal = createCalendar();
  while (scanf("%[^\n]\n", line) != EOF) {
    char *output = filterCalendarEvent(line, cal);
    if (output == NULL) {
      continue;
    }
    printf("%s\n", output);
    free(output);
    line = malloc(sizeof(char) * 100);
  }
  free(line);
  freeCalendar(cal);
  return 0;
}
