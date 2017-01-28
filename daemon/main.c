#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>

#include "tree.h"
#include "addrconverter.h"

#define BUFF_SIZE 65536
#define IPLEN 15

int main(unsigned int argc, char** argv)
{
	int raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	//int raw_icmp_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	//int raw_udp_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

	if(raw_socket < 0)
        {
                printf("Socket opening error!\n");
                return 1;
        }
	//TODO CLI interface
	if(argc > 1)
	{
		printf("Binding to %s\t\t",argv[1]);
		int res = setsockopt(raw_socket, SOL_SOCKET, SO_BINDTODEVICE, argv[1], strlen(argv[1]));
		if(res == 0)
		{
			printf("OK\n");
		}
		else
		{
			printf("FAILED\n");
			printf("Listening all devices for default.");
		}
	}
	unsigned char* buff = (unsigned char*)malloc(BUFF_SIZE);
	memset(buff,0,BUFF_SIZE);
	tree* ip_tree_root = create_node(0xFFFF0000);
	tree* ip_tree_node;
	struct sockaddr saddr;
	unsigned int addr_size = sizeof(saddr);
	int packets = 0;
	while(1)
	{
		int rec_size = recvfrom(raw_socket, buff, BUFF_SIZE, 0, &saddr, &addr_size);
		unsigned long long_ip = addr_to_long(&saddr);
		ip_tree_node = find_node(ip_tree_root, long_ip);
		ip_tree_node->_count += 1;
		system("clear");
		print_tree(ip_tree_root);
		if(rec_size < 0)
		{
			close(raw_socket);
			printf("Error in receivig file from socket.\nExiting.\n");
			break;
		}
	}
	return 0;
}
