DIR = client
OBJECT_CODES_CLIENT = $(DIR)/main.o $(DIR)/client_conn.o

snifferctl: $(OBJECT_CODES_CLIENT)
	gcc -o $@ $(OBJECT_CODES_CLIENT)

$(DIR)/main.o: $(DIR)/main.c
	gcc -c -o $@ $(DIR)/main.c

$(DIR)/client_conn.o: $(DIR)/client_conn.c $(DIR)/client_conn.h
	 gcc -c -o $@ $(DIR)/client_conn.c

clean_client:
	rm $(DIR)/*.o
