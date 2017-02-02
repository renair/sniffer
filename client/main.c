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

int main(int argc, char** argv)
{
	int pid = get_sniffer_pid();
	if(argc > 1)
	{
		unsigned int conn = conn_init_pipe();
		char* pipe = conn_attach_buffer(NULL);
		if(strcmp(argv[1],"start") == 0)
		{
			if(pid)
			{
				printf("Sniffer was already started!\n");
				return 0;
			}
			system("./snifferd &");
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
				}
				conn_get_data(data,20);
				printf("Total cathed packets:%s\n", data);
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
				kill(pid, SIGTERM);
				char buf[25];
				sprintf(buf, "./snifferd %s &", argv[3]);
				system(buf);
			}
			else if(argc == 3)
			{
				printf("Listening all devices.\n");
				kill(pid, SIGTERM);
				system("./snifferd &");
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
				printf("Statistics for :%s\n",argv[2]);
				char* data = (char*)malloc(512);
				data[0] = 's';
				strcpy(data+1, argv[2]);
				conn_set_data(data, 512, SERVER_MARKER);
				while(!conn_data_present(CLIENT_MARKER))
				{
					sleep(1);
				}
				conn_get_data(data, 512);
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
