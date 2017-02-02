CLIENT_DIR = client
OBJECT_CODES_CLIENT = $(CLIENT_DIR)/main.o $(CLIENT_DIR)/client_conn.o $(CLIENT_DIR)/tree.o  $(CLIENT_DIR)/addrconverter.o

snifferctl: $(OBJECT_CODES_CLIENT)
	gcc -o $@ $(OBJECT_CODES_CLIENT)

$(CLIENT_DIR)/main.o: $(CLIENT_DIR)/main.c
	gcc -c -o $@ $(CLIENT_DIR)/main.c

$(CLIENT_DIR)/client_conn.o: $(CLIENT_DIR)/client_conn.c $(CLIENT_DIR)/client_conn.h
	gcc -c -o $@ $(CLIENT_DIR)/client_conn.c

$(CLIENT_DIR)/tree.o: $(CLIENT_DIR)/tree.c
	gcc -c -o $@ $(CLIENT_DIR)/tree.c

$(CLIENT_DIR)/addrconverter.o: $(CLIENT_DIR)/addrconverter.c
	gcc -c -o $@ $(CLIENT_DIR)/addrconverter.c

clean_client:
	rm $(CLIENT_DIR)/*.o
