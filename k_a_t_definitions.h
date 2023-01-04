#ifndef K_DEFINITIONS_H
#define	K_DEFINITIONS_H

#include <pthread.h>

#ifdef	__cplusplus
extern "C" {
#endif



typedef struct data {
    int socket;
} DATA;

void printError(char *str);

#ifdef	__cplusplus
}
#endif

#endif	/* K_DEFINITIONS_H */

