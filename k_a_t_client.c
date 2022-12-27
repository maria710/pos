#include "k_a_t_definitions.h"
#include "k_a_t_client.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>

void nakresliObesenca(int zivoty) {
    switch (zivoty) {
        case 0:
            printf("------------------\n"
                   "|           |\n"
                   "|          ( )\n"
                   "|         --|--\n"
                   "|           |\n"
                   "|          / \\\n"
                   "|\n"
                   "|\n"
                   "------------------\n");
            break;
        case 1:
            printf("------------------\n"
                   "|           |\n"
                   "|          ( )\n"
                   "|         --|--\n"
                   "|           |\n"
                   "|          / \n"
                   "|\n"
                   "|\n"
                   "------------------\n");
            break;
        case 2:
            printf("------------------\n"
                   "|           |\n"
                   "|          ( )\n"
                   "|         --|--\n"
                   "|           |\n"
                   "|\n"
                   "|\n"
                   "|\n"
                   "------------------\n");
            break;
        case 3:
            printf("------------------\n"
                   "|           |\n"
                   "|          ( )\n"
                   "|         --|\n"
                   "|\n"
                   "|\n"
                   "|\n"
                   "|\n"
                   "------------------\n");
            break;
        case 4:
            printf("------------------\n"
                   "|           |\n"
                   "|          ( )\n"
                   "|\n"
                   "|\n"
                   "|\n"
                   "|\n"
                   "|\n"
                   "------------------\n");
            break;
        case 5:
            printf("------------------\n"
                   "|           |\n"
                   "|\n"
                   "|\n"
                   "|\n"
                   "|\n"
                   "|\n"
                   "|\n"
                   "------------------\n");
            break;
    }
}

void * clientHra(void * data) {

    DATA *d = (DATA * )data;

    printf("\n**************************************************************************\n");
    printf("HRA OBESENEC ZAČALA! DRUHÝ HRÁČ VYMÝŠĽA SLOVO, KTORÉ MÁŠ HÁDAŤ\n");
    printf("**************************************************************************\n\n");

    char buffer[2];

    int dlzkaSlova;
    read(d->socket, &dlzkaSlova, sizeof(dlzkaSlova));
    printf("Dlzka slova je %d", dlzkaSlova);

    char hadaneSlovo[dlzkaSlova];
    int pocetZivotov;

    read(d->socket, hadaneSlovo, sizeof(hadaneSlovo));

    while (1 == 1) {

        bzero(buffer, sizeof(buffer));
        char pismenko;
        printf("\n\n");
        int koniec = 1;

        for (int i = 0; i < sizeof(hadaneSlovo); ++i) {
            if(hadaneSlovo[i] == '_') {
                koniec = 0;
            }
            printf("%c", hadaneSlovo[i]);
        }
        printf("\n");

        if(koniec == 1) {
            printf("Vyhral si!\n");
            break;
        }

        printf("\n");
        printf("Zadaj jedno pismenko: ");
        scanf(" %c", &pismenko);

        buffer[0] = pismenko;
        write(d->socket, buffer, sizeof(buffer));
        read(d->socket, hadaneSlovo, sizeof(hadaneSlovo));

        read(d->socket, &pocetZivotov, sizeof (pocetZivotov));
        printf("Pocet zivotov je: %d\n", pocetZivotov);
        nakresliObesenca(pocetZivotov);

        if (pocetZivotov == 0) {
            printf("Prehral si!\n");
            break;
        }
    }
}


int client(int argc, char *argv[]) {
    if (argc < 3) {
        printError("Klienta je nutne spustit s nasledujucimi argumentmi: adresa port pouzivatel.");
    }
    
    //ziskanie adresy a portu servera <netdb.h>
    struct hostent *server = gethostbyname(argv[2]);
    if (server == NULL) {
        printError("Server neexistuje.");
    }
    int port = atoi(argv[3]);
    
    //vytvorenie socketu <sys/socket.h>
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printError("Chyba - socket.");        
    }
    
    //definovanie adresy servera <arpa/inet.h>, na aky server sa idem pripajat
    struct sockaddr_in serverAddress;
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(port);

    if (connect(sock,(struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        printError("Chyba - connect.");        
    }

    //-------------------SPOJENIE NADVIAZANE------------------------

	//inicializacia dat - socket
    DATA data = {
           sock
    };

    //vytvorenie vlakna pre zapisovanie dat do socketu <pthread.h>
    pthread_t thread;
    pthread_create(&thread, NULL, &clientHra, &data);

	//pockame na skoncenie zapisovacieho vlakna <pthread.h>
	pthread_join(thread, NULL);
	data_destroy(&data);

    //uzavretie socketu <unistd.h>
    close(sock);
    
    return (EXIT_SUCCESS);
}
