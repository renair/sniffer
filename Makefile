.PHONY: all install clean

include daemon/Makefile
include client/Makefile

all: snifferd snifferctl

install:
	cp service_template /etc/init.d/snifferd
	chmod a+x /etc/init.d/snifferd
	cp snifferctl /bin/snifferctl
	chmod a+x /bin/snifferctl
	cp snifferd /sbin/snifferd
	chmod a+x /sbin/snifferd
	mkdir -p /sbin/dumps
	update-rc.d snifferd enable

clean: clean_daemon clean_client
