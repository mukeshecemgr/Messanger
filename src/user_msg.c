#include "common.h"
#include "users.h"
#include "message.h"
int user_handle_response_message(void *msg,void *ctx)
{
}
int user_handle_notification_msg(void *msg,void *ctx)
{
}

int prepare_send_request_msg(user_info_t *user_info)
{

}

int user_handle_chat_termination_notification(void *data,void *ctx)
{
}
int user_notify_for_session_termination(void *data,void *ctx)
{
	
}

int prepare_and_send_req_msg(connection_info_t *ctrl_conn,user_info_t *me)
{
	unsigned char buff[2048] = {0};
	int len  = 0;
	user_req_msg_t *req = NULL;

	req = (user_req_msg_t *)malloc(sizeof(user_req_msg_t));
	memset(req,0,sizeof(user_req_msg_t));
	
	strcpy(req->name,"Mukesh");
	req->age = 30;
	req->sex = 'M';
	
	if ( 0 != Encode_MSG((void *)req,buff,&len,REQUEST))
	{
		printf("Failed to encode REQUEST message\n");
		return 1;
	}

	if ( 0 > (sendto(ctrl_conn->sd,buff,100,0,(struct sockaddr *)&ctrl_conn->srvr,
					sizeof(struct sockaddr))) )
	{
		printf("Failed : to send request message to server\n");
		return 1;
	}

}

int ctrl_recv_cb(int len, char *data, void *ctx)
{
	return 0;
}

