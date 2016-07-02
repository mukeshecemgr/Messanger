#include "common.h"
#include "server.h"
#include "message.h"
#include "utils.h"
server_ctx_t *srvr_ctx;

int servr_ctrl_rx_cb(int len, unsigned char *data, void *ctx)
{
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
	if ( 0 != task_mq_send(SERVER_MASTER_MQ,srvr_ctx->server_master_mq_id,
					(char *)&ipc,sizeof(ipc_header_t),0))
	{
		printf("[%d][%s]Failed to post into mq\n",__LINE__,__FUNCTION__);
		return;
	}
	
}

void start_server(connection_info_t *conn)
{
	conn->msg_cb = servr_ctrl_rx_cb;

	if ( 0 > (conn->sd = socket(AF_INET,SOCK_DGRAM,0)))
	{
		perror("socket()");
		exit(1);
	}
	conn->srvr.sin_family = AF_INET;
	conn->srvr.sin_port = htons(11111);
	conn->srvr.sin_addr.s_addr = inet_addr("192.168.1.1");

	if ( 0 > (bind(conn->sd,(struct sockaddr *)&conn->srvr,
					sizeof(struct sockaddr))))
	{
		perror("bind()");
		exit(1);
	}
/*
	conn->rx_qid = task_mq_create(100,sizeof(ipc_header_t),0,"Master_MQ");
	if ( 0 > conn->rx_qid)
	{
		printf("[%d][%s]Failed to create Master message Q\n",__LINE__,__FUNCTION__);
		return;
	}
*/
	//sleep(1);
	if ( 0 != task_spawn("Server_rx",25,1600000,
                                          (PFN)server_ctrl_rx_task,(void *)conn))
	{
		printf("[%d][%s]Failed to create thread\n",__LINE__,__FUNCTION__);
		return;
	}
	

}


void server_master_task(void *data)
{
	ipc_header_t ipc_msg;
	master_ipc_t *msg;
	int status = 0;

	while(1)
	{
		status = task_mq_receive(SERVER_MASTER_MQ,srvr_ctx->server_master_mq_id,
							(char *)&ipc_msg,sizeof(ipc_header_t),(int )-1);

		if( status == 1)
			continue;
		msg = (master_ipc_t *)ipc_msg.data;
		switch(ipc_msg.type)
		{
			case REQUEST:
				server_handle_user_req_msg(msg->data,msg->ctx);
				break;
			case TERMINATION_REQ:
				server_handle_user_termination_msg(msg->data,msg->ctx);
				break;
			default:
				printf("[%d][%s]Wrong message type received : %d\n",
								__LINE__,__FUNCTION__,ipc_msg.type);
		}
	}
		

}

void server_init()
{
	args_t args;
	server_ctx_t *ctx = NULL;

	ctx = (server_ctx_t *)malloc(sizeof(server_ctx_t));
	memset(ctx,0,sizeof(server_ctx_t));

	ctx->users_connected = (slist_t *)malloc(sizeof(slist_t));
	memset(ctx->users_connected,0,sizeof(slist_t));

	ctx->serv_conn = (connection_info_t *)malloc(sizeof(connection_info_t));
	memset(ctx->serv_conn,0,sizeof(connection_info_t));
	
	start_server(ctx->serv_conn);	

	ctx->server_master_mq_id = task_mq_create(100,sizeof(ipc_header_t),0,SERVER_MASTER_MQ);
	if ( 0 > ctx->server_master_mq_id)
	{
		printf("[%d][%s]Failed to create Master message Q\n",__LINE__,__FUNCTION__);
		return;
	}
	
	memset(&args,0,sizeof(args_t));

	if ( 0 != task_spawn("Server_Master",25,1600000,
							(PFN)server_master_task,&args))
	{
		printf("[%d][%s]Failed to create thread\n",__LINE__,__FUNCTION__);
		return;
	}
	

	srvr_ctx = ctx;
	
}

