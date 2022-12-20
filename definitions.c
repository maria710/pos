#include "definitions.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

char *endMsg = "koniec";

void printError(char *str) {
    if (errno != 0) {
		perror(str);
	}
	else {
		fprintf(stderr, "%s\n", str);
	}
    exit(EXIT_FAILURE);
}
