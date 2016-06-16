#ifndef _MESSAGE_H
#define _MESSAGE_H

typedef enum message_type
{
	REQUEST = 1,
	RESPONSE,
	NOTIFICATION_REQ,
	NOTIFICATION_RES,
	TERMINATION_REQ,
	TERMINATION_RES,
}message_type_e;

typedef struct user_req_msg_
{
	char name[100];//M
	int age;//O
	char sex;//M
	char mob[11];//O
	char location[30];//M
}user_req_msg_t;

typedef struct user_res_msg_
{
	char name[100];//M
	int data_port;
}user_res_msg_t;

typedef struct notification_
{
	char online_user[1024];
	int online_user_port;
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
