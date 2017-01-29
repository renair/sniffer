#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#include "client_conn.h"

int main(int argc, char** argv)
{
	if(argc > 1)
	{
		unsigned int conn = conn_init_pipe();
		char* pipe = conn_attach_buffer(NULL);
		if(strcmp(argv[1],"start") == 0)
		{
			printf("Starting sniffer!\n");
			return 0;
		}
		if(strcmp(argv[1],"stop") == 0)
		{
			printf("Stop sniffer!\n");
			conn_set_data("stop",strlen("stop")+1,SERVER_MARKER);
			//char buf[50];
			//conn_get_data(buf,50);
			//printf("%s\n",buf);
			return 0;
		}
		if(strcmp(argv[1],"show") == 0)
 		{
			if(argc == 4)
			{
				unsigned long addr = 10;//ip_to_long();
				printf("packets for %s: %lu\n",argv[2], addr);
			}
			else if(argc == 3)
			{
				printf("Print total amount\n");
			}
			else
			{
				printf("Usage heare\n");
			}
			return 0;
		}
		if(strcmp(argv[1],"select") == 0)
		{
			if(argc == 4)
			{
				printf("Set up interface %s\n", argv[3]);
			}
			else if(argc == 3)
			{
				printf("Listening all devices.\n");
			}
			else
			{
				printf("Usage heare.\n");
			}
			return 0;
		}
		if(strcmp(argv[1],"stat") == 0)
		{
			if(argc == 3)
			{
				printf("Statisti for :%s\n",argv[2]);
			}
			else if(argc == 2)
			{
				printf("Statistics for all interfaces:\n");
			}
			else
			{
				printf("Usage heare.\n");
			}
		}
	}
	else
	{
		printf("Usage heare\n");
	}
	return 0;
}
