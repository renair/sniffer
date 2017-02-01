#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<net/ethernet.h>
#include<linux/if_packet.h>

#include "tree.h"
#include "addrconverter.h"
#include "client_conn.h"

#define BUFF_SIZE 65536
#define IPLEN 15

int raw_socket;
tree* ip_tree_root;
char* interface;

void exit_handler(int signum);

int main(unsigned int argc, char** argv)
{
	interface = "eth0";
	//program intialization part
	raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	conn_init_pipe();
	char* conn_buffer = conn_attach_buffer();
	signal(SIGTERM, exit_handler);
	//begin do something
	if(raw_socket < 0)
        {
                printf("Socket opening error!\n");
                return 1;
        }
	save_sniffer_pid();
	//TODO binding to device
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

	ip_tree_root = load_tree(interface);
	tree* ip_tree_node;
	unsigned char packet[BUFF_SIZE];
	struct ether_header* eth = (struct ether_header*)packet;
	struct iphdr* ip = (struct iphdr*)(packet+sizeof(struct ether_header));
	while(1)
	{
		int len = read(raw_socket, packet, BUFF_SIZE);
		if(len)
		{
			char sip[15];
			//char dip[15];
			unsigned long source = ip->saddr;
			//unsigned long dest = ip->daddr;
			if(long_to_ip(source, sip, 15) != NULL)
			{
				ip_tree_node = find_node(ip_tree_root, source);
				ip_tree_node->_count += 1;
			}
		}
//		if(conn_data_present(SERVER_MARKER))
//		{
//			char command[50];
//			conn_get_data(command, 50);
//			printf("Command %c\t data:%s\n",command[0],command);
//			if(strcmp(command, "stop") == 0)
//			{
//				break;
//			}
//		}
	}
	return 0;
}


void exit_handler(int signum)
{
	printf("Exiting...\n");
	close(raw_socket);
	dump_tree(interface, ip_tree_root);
	clean_tree(ip_tree_root);
	remove_sniffer_pid();
	exit(0);
}
