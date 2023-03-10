#include "k_a_t_definitions.h"
#include "k_a_t_server.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

int pridajPismenkoAkUzHadal(char array[], char c) {
    int j = 0;
    for (int i = 0; array[i] != '\0'; i++) {
        if (array[i] == c) {
            return 1;  // uz hadal dane pismenko
        }
        j = i;
    }
    array[j] = c;  // pridame pismenko na koniec pola
    array[j + 1] = '\0';
    return 0;
}

void *serverHra(void *data) {

    DATA *d = (DATA *) data;

    int pocetZivotov = 5;

    printf("\n**************************************************************************\n");
    printf("HRA OBESENEC ZAČALA! VYMYSLI SLOVO, KTORÉ BUDE TVOJ OPONENT HÁDAŤ! \n");
    printf("**************************************************************************\n\n");

    char slovo[20];
    printf("Vymyslené slovo: ");
    fgets(slovo, 20, stdin);
    printf("Tvoj oponent háda slovo: %s\n", slovo);

    strtok(slovo, "\n");
    int dlzkaSlova = strlen(slovo);
    printf("Dlzka slova je: %d\n", dlzkaSlova);

    char hadaneSlovo[dlzkaSlova];
    for(int i = 0; i < dlzkaSlova; i++) {
        hadaneSlovo[i] = '_';
    }

    write(d->socket,&dlzkaSlova, sizeof(dlzkaSlova));
    write(d->socket, hadaneSlovo, sizeof(hadaneSlovo));

    char buffer[2];
    int pocetUhadnutych = 0;
    char hadanePismena[100] = {};

    while(1 == 1) {
        int uhadol = 0;
        bzero(buffer, sizeof(buffer));

        read(d->socket, buffer, sizeof(buffer));
        printf("Hrac zadal pismenko: %c ", buffer[0]);

        int hadane = 0;
        hadane = pridajPismenkoAkUzHadal(hadanePismena, buffer[0]);

        if(hadane == 1) {
            printf(", pismenko uz bolo hadane. ");
        } else {
            for (int i = 0; i < dlzkaSlova; ++i) {
                if (slovo[i] == buffer[0]) {
                    hadaneSlovo[i] = buffer[0];
                    uhadol = 1;
                    pocetUhadnutych++;
                }
            }

            if(uhadol == 0) {
                pocetZivotov--;
                printf(", pismenko sa v slove NENACHADZA.");
            } else {
                printf(", pismenko sa v slove NACHADZA.");
            }
        }
        printf("Pocet uhadnutych: %d\n", pocetUhadnutych);

        write(d->socket, hadaneSlovo, sizeof(hadaneSlovo));
        write(d->socket, &pocetZivotov, sizeof(pocetZivotov));

        if(pocetUhadnutych == dlzkaSlova) {
            printf("Uhadol cele slovo! Prehral si.\n");
            break;
        }

        if(pocetZivotov <= 0 ) {
            printf("Dosli mu zivoty. Vyhral si!\n");
            break;
        }
    }
}

int server(int argc, char *argv[]) {
    if (argc < 3) {
        printError("Sever je nutne spustit s nasledujucimi argumentmi: port pouzivatel.");
    }
    int port = atoi(argv[2]);

    //vytvorenie TCP socketu <sys/socket.h>
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); //TCP sockety
    if (serverSocket < 0) { //zlyhanie vracia zapornu hodnotu
        printError("Chyba - socket.");
    }

    //definovanie adresy servera <arpa/inet.h>
    struct sockaddr_in serverAddress, clientAddress;

    bzero((char*)&serverAddress, sizeof(serverAddress)); //musime najskor vynulovat
    serverAddress.sin_family = AF_INET;         //internetove sockety
    serverAddress.sin_addr.s_addr = INADDR_ANY; //prijimame spojenia z celeho internetu, ip adresy
    serverAddress.sin_port = htons(port);       //nastavenie portu, s - short

    //prepojenie adresy servera so socketom <sys/socket.h>
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError("Chyba - bind.");
    }

    //server bude prijimat nove spojenia cez socket serverSocket <sys/socket.h>
    listen(serverSocket, 10); //pasivny soket, kolko sa moze na server pripojit klientov v jednom okamihu

    //server caka na pripojenie klienta <sys/socket.h>
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);

    //uzavretie pasivneho socketu <unistd.h>
    close(serverSocket);
    if (clientSocket < 0) {
        printError("Chyba - accept.");
    }

    //------------------------------------------

    //inicializacia dat - socket
    DATA data = {
            clientSocket
    };

    //vytvorenie vlakna pre zapisovanie dat do socketu <pthread.h>
    pthread_t thread;
    pthread_create(&thread, NULL, &serverHra, &data);

    //pockame na skoncenie zapisovacieho vlakna <pthread.h>
    pthread_join(thread, NULL);

    printf("\n**************************************************************************\n");
    printf("HRA OBESENEC SKONČILA\n");
    printf("**************************************************************************\n\n");

    //uzavretie socketu klienta <unistd.h>
    close(clientSocket);

    return (EXIT_SUCCESS);
}

