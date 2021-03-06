#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define TRUE 1
#define FALSE 0
#define USER 	2
#define SERVER 	1
#define ADMIN 	3

#define CONTROL_PORT 11111


typedef struct arguments_
{
	int arg1;
	int arg2;
	int arg3;
	int arg4;
}args_t;


typedef int (*msg_recv_cb)(int,unsigned char *,void *);

typedef struct connection_info_
{
	int rx_qid;
	int serv_port;
	int sd;
	char serv_ip[16];
	char client_ip[16];
	int client_port;
	char interface[7];
	struct sockaddr_in srvr;
	struct sockaddr_in clnt;
	msg_recv_cb msg_cb;
}connection_info_t;

/*
typedef struct server_info_
{
	int sd;
	char *ip;
	int port;
	struct sockaddr_in *ser_in;
}server_info_t;

typedef struct client_
{
	int cd;
	char ip[16];
	int port;
} client_t;
*/
typedef struct ipc_header_
{
    int type;          /* IPC Message type */
	unsigned short len;             /* length of data */
	unsigned short sender_id;       /*Msg sender ID */
	unsigned int  tx_id;           /* Transaction ID */
	void *data;                 /* Actual data to be sent */
	void (*free_cb) (void *);   /*callback function to free the data part */
	char *mem_pool;
}ipc_header_t;

typedef struct slist_
{
	struct slist_ *next;
	void *node;
}slist_t;



typedef struct master_ipc_ {
        int type;
        void *data;
        void *ctx;
        int len;
}master_ipc_t;


void user_init(void);
void server_init(void);
void admin_task(void);


#endif
