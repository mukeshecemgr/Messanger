include "common.h"
#include <pthread.h>


void user_msg_tx_worker(void *data)
{
	args_t *args = (args_t *)data;
	server_info_t *sr = (server_info_t *)args->arg1;
	online_userinfo_t *friend = (online_userinfo_t *)args->arg2;
	user_info_t *me = (user_info_t *)args->arg3;
	char msg_box[1024*2] = {0};

	while(1)
	{
		printf("%s:",me->name);
		scanf("%s",msg_box);
		if ( (sendto(sr->sd,msg_box,sizeof(msg_box),0,sr,sizeof(struct sockaddr))) < 0)
		{
			perror("sendto()");
			break;
		}
	}
}


	
}
