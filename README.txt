Aplikacia sluzi na (asynchronnu) vymenu textovych sprav medzi dvomi pouzivatelmi.
Asynchronnost je zabezpecena vyuzitim vlakien a funkcie select.

Preklad servera:
	gcc k_a_t_definitions.c k_a_t_server.c -o k_a_t_server -pthread
Preklad klienta:
	gcc k_a_t_definitions.c k_a_t_client.c -o k_a_t_client -pthread

Spustenie servera:
	./k_a_t_server server 10000
Spustenie klienta:
	./k_a_t_client client localhost 10000
	
