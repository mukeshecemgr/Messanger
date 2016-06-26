#include "message.h"
#include "common.h"
#include "users.h"
#include "server.h"

void user_ctrl_rx_task(void *data)
{
	args_t *args = (args_t *)data;
	connection_info_t *ctrl = (connection_info_t *)args->arg1;
	struct sockaddr_in serv;
	int sz =0,len = sizeof(serv);
	unsigned char ctrl_buff[1024]  ={0};
	while(1)
	{
		sz = recvfrom(ctrl->sd,ctrl_buff,sizeof(ctrl_buff),0,(struct sockaddr *)&serv,
						&len);
		if ( sz < 0)
			continue;
		else
			ctrl->msg_cb(sz,ctrl_buff,&serv);

	}
	return;
}

void user_data_rx_task(void *data)
{
	return;
}

void server_ctrl_rx_task(void *data)
{
	long unsigned int sz = 0;
	//args_t *args = (args_t *)data;
	connection_info_t *conn = (connection_info_t *)data;
	if ( NULL == conn)
        {
	    printf("[%d][%s]NULL Conn info received\n",__LINE__,__FUNCTION__);
	    return ;
        }
	struct sockaddr_in clnt;
	int len = sizeof(clnt);
	unsigned char buff[1024] = {0};

	while(1)
	{
		sz = recvfrom(conn->sd,buff,sizeof(buff),0,(struct sockaddr*)&clnt,&len);

		if ( sz < 0)
			continue;
		else
			conn->msg_cb(len,buff,(void *)&clnt);
	}
	return;
}
