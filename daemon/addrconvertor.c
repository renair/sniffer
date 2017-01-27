#include<arpa/inet.h>

#include "addrconvertor.h"

//return ulong representation of ip and string representation
unsigned long addr_to_long(struct sockaddr* sa)
{
        return (((struct sockaddr_in*)sa)->sin_addr).s_addr;
}

char* long_to_ip(unsigned long num, char* ip, unsigned int len)
{
        struct in_addr addr;
        addr.s_addr = num;
        inet_ntop(AF_INET, &addr, ip, len);
        return ip;
}

unsigned long ip_to_long(char* ip)
{
        struct in_addr addr;
        inet_pton(AF_INET, ip, &addr);
        return addr.s_addr;
}

