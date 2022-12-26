#include "k_a_t_definitions.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

int server(int argc, char *argv[]) {
    if (argc < 3) {
        printError("Sever je nutne spustit s nasledujucimi argumentmi: port pouzivatel.");
    }
    int port = atoi(argv[2]);
    char *userName = argv[3];

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

    //-------------------SPOJENIE NADVIAZANE------------------------

    printf("\n***********************************************\n\n");
    printf("HRA OBESENEC ZAČALA! VYMYSLI SLOVO, KTORÉ BUDE TVOJ OPONENT HÁDAŤ! \n");
    printf("Pre ukončenie napíš: koniec\n");
    char slovo[100];
    printf("Vymyslené slovo: ");
    scanf("%s", slovo);
    printf("Tvoj oponent háda slovo: %s\n", slovo);

    //inicializacia dat zdielanych medzi vlaknami
    DATA data;
    data_init(&data, userName, clientSocket);

    //vytvorenie vlakna pre zapisovanie dat do socketu <pthread.h>
    pthread_t thread;
    pthread_create(&thread, NULL, data_writeData, (void *) &data);


    //v hlavnom vlakne sa bude vykonavat citanie dat zo socketu
    data_readData((void *) &data);

    //pockame na skoncenie zapisovacieho vlakna <pthread.h>
    pthread_join(thread, NULL);
    data_destroy(&data);

    //uzavretie socketu klienta <unistd.h>
    close(clientSocket);

    return (EXIT_SUCCESS);
}