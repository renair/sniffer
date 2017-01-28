#ifndef ADDRCONVERTOR_H
#define ADDRCONVERTOR_H

#include<netinet/in.h>

unsigned long addr_to_long(struct sockaddr*);
char* long_to_ip(unsigned long, char*, unsigned int);
unsigned long ip_to_long(char*);

#endif
