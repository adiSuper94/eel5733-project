#include "Filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *line = malloc(sizeof(char) * 100);
    while(scanf("%[^\n]\n", line) != EOF){
        struct CalendarEvent *event = getCalendarEvent(line);
        free(line);
        line = malloc(sizeof(char) * 100);
        if(event != NULL){
            printf("%s,%s,%s\n", event->date, event->time, event->location);
            free_event(event);
        }
    }
    free(line);
    return 0;
}
