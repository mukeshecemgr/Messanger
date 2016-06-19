#include "common.h"
#include "users.h"
#include "message.h"

extern user_ctx_t *user;

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
	unsigned char buff[1024] = {0};
	int len  = 0;
	user_req_msg_t *req = NULL;

	req = (user_req_msg_t *)malloc(sizeof(user_req_msg_t));
	memset(req,0,sizeof(user_req_msg_t));
	
	memcpy(req->name,"Mukesh",strlen("Mukesh")+1);
	req->age = 30;
	req->sex = 'M';
	memcpy(req->mob,"9884465584",strlen("9884465584")+1);
	memcpy(req->location,"Chennai",strlen("Chennai")+1);

	if ( 0 != Encode_MSG((void *)req,buff,&len,REQUEST))
	{
		printf("Failed to encode REQUEST message\n");
		return 1;
	}
	printf("Enoded Buffer : %p\n",buff);

	if ( 0 > (sendto(ctrl_conn->sd,buff,len,0,(struct sockaddr *)&ctrl_conn->srvr,
					sizeof(struct sockaddr))) )
	{
		printf("Failed : to send request message to server\n");
		return 1;
	}
	return 0;

}

int ctrl_recv_cb(int len,unsigned char *data, void *ctx)
{
          printf("User Received message \n");
          ipc_header_t ipc;
          master_ipc_t *msg = (master_ipc_t *)malloc(sizeof(master_ipc_t));
          int mt = 0;
          void *pvalue = NULL;
          if ( 0 != Decode_MSG(data,&len,&mt,&pvalue))
          {
                  printf("[%d][%s]failed to decode message\n",__LINE__,__FUNCTION__);
                  return;
          }

          msg->data = pvalue;
          msg->ctx = ctx;

          /*Send to master task, Where all control message will be
          * processes */
          ipc.type = mt;
          ipc.len = sizeof(ipc_header_t);
          ipc.data = msg;
          if ( 0 != task_mq_send(USER_MASTER_Q,user->master_qid,
                                          (char *)&ipc,sizeof(ipc_header_t),0))
          {
                  printf("[%d][%s]Failed to post into mq\n",__LINE__,__FUNCTION__);
                  return;
          }

	return 0;
}

