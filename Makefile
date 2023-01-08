LDLAGS=-pthread
CC = gcc

all: main

klient:
	./main client localhost 18901

server:
	./main server 18901

clean:
	rm -f *.o *~ main

.PHONY: all clear

main: main.o k_a_t_client.o k_a_t_server.o k_a_t_definitions.o
	$(CC) $^ -o $@ $(LDLAGS)

%.o: %.c
	$(CC) $< -c


main.o: k_a_t_client.h k_a_t_server.h

k_a_t_definitions.o: k_a_t_definitions.h

k_a_t_client.o: k_a_t_client.h k_a_t_definitions.h

k_a_t_server.o: k_a_t_server.h k_a_t_definitions.h