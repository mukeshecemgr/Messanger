#ifndef _USERS_H
#define _USERS_H

#define USER_MASTER_Q "/user_master"

//typedef int (*msg_recv_cb)(int len,char *data, void *ctx);

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

void init_ctrl_path(connection_info_t *);
int ctrl_recv_cb(int, char *,void *);
void user_ctrl_rx_task(void *);
void init_usr_msg_data_path();

#endif
