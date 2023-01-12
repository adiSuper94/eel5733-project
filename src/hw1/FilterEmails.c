#include "Filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *line = malloc(sizeof(char) * 100);
    while(scanf("%[^\n]\n", line) != EOF){
        char *res = filterEmail(line);
        free(line);
        line = malloc(sizeof(char) * 100);
        if(res != NULL){
            printf("%s\n", res);
            free(res);
        }
    }
    free(line);
    return 0;
}
