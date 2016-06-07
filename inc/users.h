#include "common.h"

typedef int (*msg_recv_cb)(int len,char *data, void *ctx);

typedef struct user_info_
{
	char name[100];
	int age;
	char sex;
	char location[100];
	char mobile_num[11];
	int data_port;
}user_info_t;

typedef struct online_userinfo_
{
	struct online_userinfo_ *next;
	char name[100];
	int age;
	char sex;
	char location[100];
	char mobile_num[11];
	int online_status;
}online_userinfo_t;

typedef struct connection_info_
{
	int port;
	int sd;
	char ip[16];
	msg_recv_cb msg_cb;
}connection_info_t;
typedef struct user_ctx_
{
	int ctrl_port;
	int data_port;
	int master_qid;
	user_info_t *me;
	slist_t *online;
	connection_info_t *ctrl_conn;
	connection_info_t *data_conn;
}user_ctx_t;

typedef struct user_ipc_ {
	int type;
	int len;
	void *data;
	void *ctx;
}user_ipc_t;
