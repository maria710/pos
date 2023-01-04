#include "k_a_t_server.h"
#include "k_a_t_client.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    if(strcmp(argv[1], "server") == 0) {
        return server(argc, argv);
    } else if (strcmp(argv[1], "client") == 0) {
        return client(argc, argv);
    } else {
        fprintf(stderr, "Nespravne argumenty\n");
        return 1;
    }
}
