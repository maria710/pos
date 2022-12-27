#include "k_a_t_server.h"
#include "k_a_t_client.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    if(strcmp(argv[1], "server") == 0) {
        printf("Som v 1\n");
        return server(argc, argv);
    } else if (strcmp(argv[1], "client") == 0) {
        printf("Som v 2\n");
        return client(argc, argv);
    } else {
        fprintf(stderr, "Nespravne argumenty\n");
        return 1;
    }
}
