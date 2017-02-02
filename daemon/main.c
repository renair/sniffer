#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/ioctl.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<net/if.h>
#include<net/ethernet.h>
#include<linux/if_packet.h>

#include "tree.h"
#include "addrconverter.h"
#include "client_conn.h"

#define BUFF_SIZE 65536

int raw_socket;
tree* ip_tree_root;
char interface[25];

void exit_handler(int signum);

int main(unsigned int argc, char** argv)
{
	if(argc > 1)
	{
		sprintf(interface, "dumps/%s",argv[1]);
	}
	else
	{
		sprintf(interface, "dumps/%s","all");
	}
	printf("Binding to device: %s\n", argv[1]);
	//program intialization part
	raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	conn_init_pipe();
	conn_attach_buffer();
	signal(SIGTERM, exit_handler);
	signal(SIGKILL, exit_handler);
	signal(SIGHUP, exit_handler);
	//begin do something
	if(raw_socket < 0)
        {
                printf("Socket opening error!\n");
                return 1;
        }
	save_sniffer_pid();
	//binding to device
	if(argc > 1)
	{
		sprintf(interface, "dumps/%s",argv[1]);
		printf("Binding to %s\t\t",argv[1]);
		struct sockaddr_ll bind_addr;
		struct ifreq ioctl_req;
		memset(&bind_addr,0,sizeof(struct sockaddr_ll));
		memset(&ioctl_req,0,sizeof(struct ifreq));
		strcpy(ioctl_req.ifr_name, argv[1]);
		if((ioctl(raw_socket, SIOCGIFINDEX, &ioctl_req)) == -1)
		{
			printf("\nBinding to %s failed!\n",argv[1]);
		}
		bind_addr.sll_family = AF_PACKET;
		bind_addr.sll_ifindex = ioctl_req.ifr_ifindex;
		bind_addr.sll_protocol = htons( ETH_P_ALL);
		if(bind(raw_socket, (struct sockaddr*)&bind_addr, sizeof(bind_addr)) == 0)
		{
			printf("OK\n");
		}
		else
		{
			printf("FAILED\n");
			printf("Listening all devices for default.");
		}
	}
	else
	{
		sprintf(interface, "dumps/all");
	}
	printf("Sniffer runned!\n");
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
		if(conn_data_present(SERVER_MARKER))
		{
			char command[25];
			conn_get_data(command, 25);
			printf("Command %c\t data:%s\n",command[0],command+1);
			if(command[0] == 'i')
			{
				unsigned long needle = ip_to_long(command+1);
				tree* needle_node = find_node(ip_tree_root, needle);
				sprintf(command,"%lu",needle_node->_count);
				conn_set_data(command, 25, CLIENT_MARKER);
				continue;
			}
			if(command[0] == 'I')
			{
				sprintf(command,"%e",count_tree(ip_tree_root));
				conn_set_data(command, 25, CLIENT_MARKER);
				continue;
			}
			if(command[0] == 's')
			{
				char local_path[512];
				sprintf(local_path, "dumps/%s", command+1);
				printf("\tLocal path: %s\n",local_path);
				FILE* f = fopen(local_path,"r");
				if(!f)
				{
					fclose(f);
					printf("\tFile %s not found!",local_path);
					conn_set_data("n",2,CLIENT_MARKER);
					continue;
				}
				fclose(f);
				dump_tree(interface, ip_tree_root);
				char* path = (char*) malloc(512);
				realpath(local_path, path);
				printf("\tServer path: %s\n", path);
				conn_set_data(path, 512, CLIENT_MARKER);
				free(path);
				continue;
			}
			if(command[0] == 'S')
			{
				dump_tree(interface, ip_tree_root);
				char* path = (char*) malloc(512);
				realpath("dumps", path);
				conn_set_data(path, 512, CLIENT_MARKER);
				free(path);
				continue;
			}
		}
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
