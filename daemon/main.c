#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define BUFF_SIZE 65536

int main(unsigned int argc, char** argv)
{
	int raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
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
	struct sockaddr saddr;
	unsigned int addr_size = sizeof(saddr);
	int packets = 0;
	while(1)
	{
		int rec_size = recvfrom(raw_socket, buff, BUFF_SIZE, 0, &saddr, &addr_size);
		++packets;
		printf("\r%d packets received\t\tLast from: %s                    ",packets, inet_ntoa(((struct sockaddr_in *)&saddr)->sin_addr));
		if(rec_size < 0)
		{
			close(raw_socket);
			printf("Error in receivig file from socket.\nExiting.\n");
			break;
		}
	}
	return 0;
}
