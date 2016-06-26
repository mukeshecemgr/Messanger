#ifndef _MESSAGE_H
#define _MESSAGE_H

typedef enum message_type
{
	REQUEST = 0,
	RESPONSE,
	NOTIFICATION,
	TERMINATION_REQ,
	TERMINATION_RES,
}message_type_e;

typedef struct contact_info {
  unsigned char c_name_id[30];
  unsigned char c_sex;
}contact_info_t;

typedef struct user_req_msg_
{
	unsigned char name[100];//M
	unsigned char age;//O
	unsigned char sex;//M
	unsigned char mob[11];//O
	unsigned char location[30];//M
	contact_info_t contacts[100];
	unsigned char contact_count;
}user_req_msg_t;



typedef struct user_res_msg_
{
	char name[100];//M
	int data_port;
}user_res_msg_t;

typedef struct online_users {
  char online_uid[30];
  char status;  // 1- Online | 2 - Offline
}online_users_t;

typedef struct notification_
{
	online_users_t online_user[1024];
	unsigned char online_user_cnt;
}notification_t;

typedef struct chat_box_
{
	char msg[1024];
}chat_box_t;

int user_handle_response_message(void *,void *);
int user_handle_notification_msg(void *, void *);
int user_handle_chat_termination_notification(void *,void *);
int user_notify_for_session_termination(void *,void *);

#endif
