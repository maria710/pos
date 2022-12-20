#include "definitions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

char* spracujData(char *data) {
    char *akt = data;
    while (*akt != '\0') {
        if (islower(*akt)) {
            *akt = toupper(*akt);
        }
        else if (isupper(*akt)) {
            *akt = tolower(*akt);
        } 
		akt++;		
    }
    return data;
}

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

int getRandomInInterval(int dolnaHranica, int hornaHranica) {
    return rand() % (hornaHranica - dolnaHranica + 1) + dolnaHranica;
}

char slova[30][20] = {
        "JABLKO",
        "POCITAC",
        "MALICKOST",
        "SPORTOVANIE",
        "PROGRAMATOR",
        "MANDARINKA",
        "GRAPEFRUIT",
        "SLNECNIK",
        "PALINDROM",
        "ZEMEGULA",
        "NARODENINY",
        "ORIESKY",
        "VOZIDLO",
        "CHOBOTNICA",
        "ZMYJA",
        "LAMBORGHINI",
        "PENICILIN",
        "GRAMOFON",
        "KOLONOSKOPIA",
        "OTORINOLARYNGOLOG",
        "RUKAVICKY",
        "FATAMORGANA",
        "UCHO",
        "VIANOCE",
        "SILVESTER",
        "PIROHY",
        "PYROTECHNIKA",
        "KLAMPIAR",
        "BLESKOZVOD",
        "SLIVKY"};

int main(int argc, char** argv) {
    if (argc < 2) {
        printError("Sever je nutne spustit s nasledujucimi argumentmi: port.");
    }
    int port = atoi(argv[1]);
	if (port <= 0) {
		printError("Port musi byt cele cislo vacsie ako 0.");
	}

    //vytvorenie TCP socketu <sys/socket.h>
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        printError("Chyba - socket.");        
    }
    
    //definovanie adresy servera <arpa/inet.h>
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;         //internetove sockety
    serverAddress.sin_addr.s_addr = INADDR_ANY; //prijimame spojenia z celeho internetu
    serverAddress.sin_port = htons(port);       //nastavenie portu
    
    //prepojenie adresy servera so socketom <sys/socket.h>
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError("Chyba - bind.");
    }
    
    //server bude prijimat nove spojenia cez socket serverSocket <sys/socket.h>
    listen(serverSocket, 10);
    
    //server caka na pripojenie klienta <sys/socket.h>
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

    //uzavretie pasivneho socketu <unistd.h>
    close(serverSocket);
    if (clientSocket < 0) {
        printError("Chyba - accept.");        
    }
	
    printf("Klient sa pripojil na server.\n");

    //----------------------------------------------SPOJENIE KLIENTA SO SERVEROM USPESNE--------------------------------------------------

    //----------------------------------------------UVODNY VYPIS SERVER--------------------------------------------------

    //TODO vygenerovanie slova z nejakeho zoznamu slov (index od 0 - 29)
    srand(time(NULL));
    int randomIndex = getRandomInInterval(0,29);
    char* slovo = slova[randomIndex];
    printf("Vygenerovane slovo : %s\n", slovo);

    //toto ma byt v inej casti kodu
    //TODO vypis zostavajucich a pouzitych pismen - z nejakeho pola ?
    printf("Zostavajuce pismenka: \n");
    printf("Použite pismenka: \n");
    //TODO vypis slova kde pismena nahradime za ciarky
    printf("SLOVO: _ _ _ _ _ _ _ _ _");
    //TODO vypis aktualneho stavu obesenca
    nakresliObesenca(5);

    //TODO cakanie na zadanie pismenka od hraca - vlakno ???
    printf("Zadaj pismenko : \n");

    //new branch commit :)

    char buffer[BUFFER_LENGTH + 1];
    buffer[BUFFER_LENGTH] = '\0';
    int koniec = 0;
    while (!koniec) {
        //citanie dat zo socketu <unistd.h>
		read(clientSocket, buffer, BUFFER_LENGTH);
        if (strcmp(buffer, endMsg) != 0) {
            printf("Klient zadal pismenko:\n%s\n", buffer);
            spracujData(buffer);
			//zapis dat do socketu <unistd.h>
			write(clientSocket, buffer, strlen(buffer) + 1);
        }
        else {
            koniec = 1;
        }
    }
    printf("Klient ukoncil komunikaciu.\n");
    
    //uzavretie socketu klienta <unistd.h>
    close(clientSocket);
    
    return (EXIT_SUCCESS);
}

