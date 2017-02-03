#@ = left part of the rule

SNIFF_PATH = daemon
OBJECT_CODES_DAEMON = $(SNIFF_PATH)/main.o $(SNIFF_PATH)/tree.o $(SNIFF_PATH)/addrconverter.o $(SNIFF_PATH)/client_conn.o

snifferd: $(OBJECT_CODES_DAEMON)
	gcc -o $@ $(OBJECT_CODES_DAEMON)

$(SNIFF_PATH)/main.o: $(SNIFF_PATH)/main.c
	gcc -c -o $@ $(SNIFF_PATH)/main.c

$(SNIFF_PATH)/tree.o: $(SNIFF_PATH)/tree.c $(SNIFF_PATH)/tree.h
	gcc -c -o $@ $(SNIFF_PATH)/tree.c

$(SNIFF_PATH)/addrconvertor.o: $(SNIFF_PATH)/addrconverter.c $(SNIFF_PATH)/addrconverter.h
	gcc -c -o $@ $(SNIFF_PATH)/addrconverter.c

$(SNIFF_PATH)/client_conn.o: $(SNIFF_PATH)/client_conn.c $(SNIFF_PATH)/client_conn.h
	gcc -c -o $@ $(SNIFF_PATH)/client_conn.c

clean_daemon: 
	rm -f $(SNIFF_PATH)/*.o
