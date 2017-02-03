#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>

#include "client_conn.h"
#include "tree.h"

#define SNIFFERD "snifferd &"
#define SNIFFERD_IFACE "snifferd %s &"

void usage(char*);

int main(int argc, char** argv)
{
	int pid = get_sniffer_pid();
	if(argc > 1)
	{
		int i = 3; //timelimit in seconds
		unsigned int conn = conn_init_pipe();
		char* pipe = conn_attach_buffer(NULL);
		if(strcmp(argv[1],"start") == 0)
		{
			if(pid)
			{
				printf("Sniffer was already started!\n");
				return 0;
			}
			system(SNIFFERD);
			return 0;
		}
		if(strcmp(argv[1],"stop") == 0)
		{
			if(pid != 0)
			{
				printf("Stoping sniffer!(PID:%d)\n", pid);
				kill(pid, SIGTERM);
			}
			return 0;
		}
		if(strcmp(argv[1],"show") == 0)
 		{
			if(argc == 4)
			{
				char data[16];
				data[0] = 'i';
				strcpy(data+1, argv[2]);
				conn_set_data(data,16,SERVER_MARKER);
				while(!conn_data_present(CLIENT_MARKER))
				{
					sleep(1);
					if(i-- == 0)
					{
						printf("Sniffer not response. It may be not started.\n");
						return 0;
					}
				}
				conn_get_data(data,16);
				printf("packets for %s: %s\n",argv[2], data);
			}
			else if(argc == 3)
			{
				char data[20];
				data[0] = 'I';
				data[1] = '\0';
				conn_set_data(data,20,SERVER_MARKER);
				while(!conn_data_present(CLIENT_MARKER))
				{
					sleep(1);
					if(i-- == 0)
					{
						printf("Sniffer not response. It may be not started.\n");
						return 0;
					}
				}
				conn_get_data(data,20);
				printf("Total cathed packets:%s\n", data);
			}
			else
			{
				usage(argv[0]);
			}
			return 0;
		}
		if(strcmp(argv[1],"select") == 0)
		{
			if(argc == 4)
			{
				printf("Set up interface %s\n", argv[3]);
				kill(pid, SIGTERM);
				char buf[25];
				sprintf(buf, SNIFFERD_IFACE, argv[3]);
				system(buf);
			}
			else if(argc == 3)
			{
				printf("Listening all devices.\n");
				kill(pid, SIGTERM);
				system(SNIFFERD);
			}
			else
			{
				usage(argv[0]);
			}
			return 0;
		}
		if(strcmp(argv[1],"stat") == 0)
		{
			if(argc == 3)
			{
				printf("Statistics for :%s\n",argv[2]);
				char* data = (char*)malloc(512);
				data[0] = 's';
				strcpy(data+1, argv[2]);
				conn_set_data(data, 512, SERVER_MARKER);
				while(!conn_data_present(CLIENT_MARKER))
				{
					sleep(1);
					if(i-- == 0)
					{
						printf("Sniffer not response. It may be not started.\n");
						return 0;
					}
				}
				conn_get_data(data, 512);
/*   */				printf("dir: %s\n",data);
				tree* root = load_tree(data);
				print_tree(root);
				clean_tree(root);
				free(data);
			}
			else if(argc == 2)
			{
				char* data = (char*)malloc(512);
				data[0] = 'S';
				data[1] = '\0';
				conn_set_data(data,512, SERVER_MARKER);
				while(!conn_data_present(CLIENT_MARKER))
				{
					sleep(1);
					if(i-- == 0)
					{
						printf("Sniffer not response. It may be not started.\n");
						return 0;
					}
				}
				conn_get_data(data,512);
				DIR* dir = opendir(data);
				struct dirent* file;
				while((file = readdir(dir)) != NULL)
				{
					if(file->d_name[0] == '.')
					{
						continue;
					}
					char fullpath[512];
					sprintf(fullpath,"%s/%s",data,file->d_name);
					tree* root = load_tree(fullpath);
					printf("\tPackets for %s:\n",file->d_name);
					print_tree(root);
					clean_tree(root);
				}
				closedir(dir);
				free(data);
			}
			else
			{
				usage(argv[0]);
			}
		}
		if(strcmp("--help",argv[1]) == 0)
		{
			usage(argv[0]);
		}
	}
	else
	{
		usage(argv[0]);
	}
	return 0;
}

void usage(char* com)
{
	printf("%s start - start sniffing packets from default iface(eth0)\n", com);
	printf("%s stop - stop sniffing and save packets dump\n", com);
	printf("%s show [ip] count - if ip set - return packets for those ip else return total amount of sniffed packets\n", com);
	printf("%s select iface [iface] - if iface set - set to those iface else set to default(eth0)\n", com);
	printf("%s stat [iface] - if iface set show total statistic for those iface else show for all ifaces(which was sniffed before)\n", com);
	printf("%s --help - print this help page\n", com);
}
