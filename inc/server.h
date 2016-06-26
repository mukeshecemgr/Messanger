#ifndef _SERVER_H
#define _SERVER_H

#include "common.h"
#include "message.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_CTRL_PORT 11111
#define SERVER_MASTER_MQ "/Master_mq"
typedef struct server_ctx_
{
	connection_info_t *serv_conn;
	slist_t *users_connected;
	int server_master_mq_id;
}server_ctx_t;

typedef struct user_details
{
  char name[100];
  int age;
  char sex;
  char mob[11];
  char location[30];

}user_details_t;

typedef struct user_conn_details
{
  user_req_msg_t *user_ctx;
  void *ctx;
  int reg_state; //0-Not Registered, 1- Registered
  slist_t *friends; //Implemet later when chatting starts successfully
}user_conn_details_t;

int servr_ctrl_rx_cb(int ,unsigned char *, void *);
void server_ctrl_rx_task(void *);

#endif
