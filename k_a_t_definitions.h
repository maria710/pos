#ifndef K_DEFINITIONS_H
#define	K_DEFINITIONS_H

#include <pthread.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define USER_LENGTH 10
#define BUFFER_LENGTH 300
extern char *endMsg;

typedef struct data {
    int socket;
} DATA;

void data_destroy(DATA *data);
void data_stop(DATA *data);
int data_isStopped(DATA *data);

void printError(char *str);

#ifdef	__cplusplus
}
#endif

#endif	/* K_DEFINITIONS_H */

