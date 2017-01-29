#ifndef CLIENT_CONN_H
#define CLIENT_CONN_H

#define SHM_KEY 45290
#define SHM_SIZE 10240
#define SNIFF_GET_IP 1
#define SNIFF_GET_ALL 2
#define SNIFF_SET_DEV 3
#define SERVER_MARKER 's'
#define CLIENT_MARKER 'c'

unsigned int conn_init_pipe();
char* conn_attach_buffer();
char conn_data_present(char marker);
void conn_get_data(char*, unsigned int);
void conn_set_data(char* buf, unsigned int size, char marker);

#endif
