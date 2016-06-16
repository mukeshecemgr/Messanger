#ifndef _SERVER_H
#define _SERVER_H

#include "common.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_CTRL_PORT 11111
#define SERVER_MASTER_MQ "/Master_mq"
typedef struct server_ctx_
{
	connection_info_t *serv_conn;
	slist_t *users_connected;
	int server_master_mq_id;
}server_ctx_t;

typedef struct master_ipc_ {
	int type;
	void *data;
	void *ctx;
	int len;
}master_ipc_t;

int servr_ctrl_rx_cb(int ,unsigned char *, void *);
void server_ctrl_rx_task(void *);

#endif
