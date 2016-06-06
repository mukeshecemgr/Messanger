#include "common.h"

void user_msg_rx_worker(void *data)
{
	args_t *args = (args_t *)data;
	server_info_t *sr = (server_info_t *)args->arg1;
	online_userinfo_t *friend = (online_user_info_t *)args2;
	user_info_t *me = (user_info_t *)args->arg3;
	struct sockaddr_in clnt;
	int sz =0,len = sizeof(clnt);
	while(1)
	{
		sz = recvfrom(sr->sd,msg_box,sizeof(msg_buf),0,(struct sockaddr *)&clnt,
						sizeof(struct sockaddr)&len);
		if ( sz < 0)
			continue;
		else
			printf("%s:",friend->name);
	}
}
