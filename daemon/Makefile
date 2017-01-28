#@ = left part of the rule

SNIFF_PATH = daemon
OBJECT_CODES = $(SNIFF_PATH)/main.o $(SNIFF_PATH)/tree.o $(SNIFF_PATH)/addrconverter.o

sniffer: $(OBJECT_CODES)
	gcc -o sniffer $(OBJECT_CODES)

$(SNIFF_PATH)/main.o: $(SNIFF_PATH)/main.c
	gcc -c -o $@ $(SNIFF_PATH)/main.c

$(SNIFF_PATH)/tree.o: $(SNIFF_PATH)/tree.c $(SNIFF_PATH)/tree.h
	gcc -c -o $@ $(SNIFF_PATH)/tree.c

$(SNIFF_PATH)/addrconvertor.o: $(SNIFF_PATH)/addrconverter.c $(SNIFF_PATH)/addrconverter.h
	gcc -c -o $@ $(SNIFF_PATH)/addrconverter.c

clean_daemon: 
	rm $(SNIFF_PATH)/*.o
