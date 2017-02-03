#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define SHM_KEY 10220
#define SHM_SIZE 1024
#define SNIFF_GET_IP 1
#define SNIFF_GET_ALL 2
#define SNIFF_SET_DEV 3
#define PID_FILE "/tmp/.sniffer4836.pid"

unsigned int shm_pipe = 0;
char* shm_data = 0;

unsigned int conn_init_pipe()
{
	return (shm_pipe = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT));
}

char* conn_attach_buffer()
{
	if(shm_pipe != 0)
	{
		shm_data = shmat(shm_pipe, NULL, 0);
		if(shm_data == (void*)-1)
		{
			switch(errno)
			{
				case EACCES: printf("\tNo acces to memory! Need root privilages.\n"); break;
				default:printf("\tUnknown error in conn_attach_buffer!\n");break;
			}
			printf("SHMAT error!\n");
			exit(1);
		}
		memset(shm_data, 0, SHM_SIZE);
		return shm_data;
	}
	return shm_data;
}

char conn_data_present(char marker)
{
	return shm_data[0] == marker ? 1 : 0;
}

void conn_get_data(char* buf, unsigned int size)
{
	size = strlen(shm_data+1) > size ? size : strlen(shm_data+1)+1;
	strncpy(buf, shm_data+1, size);
	shm_data[0] = 0;
}

void conn_set_data(char* buf, unsigned int size, char marker)
{
	strncpy(shm_data+1, buf, size);
	shm_data[0] = marker;
}

int get_sniffer_pid()
{
	FILE* pid_file = fopen(PID_FILE,"r");
	if(pid_file == NULL)
	{
		return 0;
	}
	int pid = 1234;
	fread(&pid, sizeof(int), 1, pid_file);
	fclose(pid_file);
	return pid;
}

void save_sniffer_pid()
{
	FILE* pid_file = fopen(PID_FILE,"w+");
	if(pid_file == NULL)
	{
		printf("Error during saving pid\n");
		return;
	}
	int pid = getpid();
	fwrite(&pid, sizeof(int), 1, pid_file);
	fclose(pid_file);
}

int remove_sniffer_pid()
{
	FILE* pid_file = fopen(PID_FILE,"r");
	if(pid_file)
	{
		fclose(pid_file);
		return (remove(PID_FILE) == 0) ? 1 : 0;
	}
	return 0;
}
