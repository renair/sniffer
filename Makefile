.PHONY: all help clean

help:
	echo "Use `make all` to build sniffer and client"

include daemon/Makefile
include client/Makefile

all: snifferd snifferctl

clean: clean_daemon clean_client
