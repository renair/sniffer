CLIENT_DIR = client
OBJECT_CODES_CLIENT = $(CLIENT_DIR)/main.o $(CLIENT_DIR)/client_conn.o

snifferctl: $(OBJECT_CODES_CLIENT)
	gcc -o $@ $(OBJECT_CODES_CLIENT)

$(DIR)/main.o: $(CLIENT_DIR)/main.c
	gcc -c -o $@ $(CLIENT_DIR)/main.c

$(DIR)/client_conn.o: $(CLIENT_DIR)/client_conn.c $(CLIENT_DIR)/client_conn.h
	 gcc -c -o $@ $(CLIENT_DIR)/client_conn.c

clean_client:
	rm $(CLIENT_DIR)/*.o
