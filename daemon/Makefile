#@ = left part of the rule

SNIFF_PATH = daemon
OBJECT_CODE = $(SNIFF_PATH)/main.o $(SNIFF_PATH)/tree.o $(SNIFF_PATH)/addrconvertor.o

sniffer: $(OBJECT_CODE)
	gcc -o sniffer $(OBJECT_CODE)

$(SNIFF_PATH)/main.o: $(SNIFF_PATH)/main.c
	gcc -c -o $@ $(SNIFF_PATH)/main.c

$(SNIFF_PATH)/tree.o: $(SNIFF_PATH)/tree.c $(SNIFF_PATH)/tree.h
	gcc -c -o $@ $(SNIFF_PATH)/tree.c

$(SNIFF_PATH)/addrconvertor.o: $(SNIFF_PATH)/addrconvertor.c $(SNIFF_PATH)/addrconvertor.h
	gcc -c -o $@ $(SNIFF_PATH)/addrconvertor.c

clean_daemon: 
	rm $(SNIFF_PATH)/*.o
