#ifndef _USERS_H
#define _USERS_H

#include <sys/stat.h>

#define USER_NAME "Mukesh"
#define SEX 'M'
#define AGE 30
#define USER_MASTER_Q "/Master_user_mq_"

#define CONTACT_FILE "Contact/contact_"
#define USER_DB "user_db/db_"

//typedef int (*msg_recv_cb)(int len,char *data, void *ctx);

typedef struct user_info_
{
	char name[100];
	int age;
	char sex;
	char location[100];
	char mobile_num[11];
	int data_port;
	int control_port;
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
int ctrl_recv_cb(int, unsigned char *,void *);
void user_ctrl_rx_task(void *);
void init_usr_msg_data_path();

#endif
