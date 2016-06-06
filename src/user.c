#include "common.h"
#include "users.h"

user_ctx_t *user = NULL;

void user_master_task(void *data)
{
	args_t args = (args_t *)data;
	user_ipc_t msg;
	int status;
	while(1)
	{
		status = task_mq_receive("user_master",user->master_qid,
									(char *)&msg,
									sizeof(ipc_header_t),
									(unsigned int) -1);
		if ( status == 1)
			continue;

		switch(msg.type)
		{
		}
	}
}

void user_init()
{
	if ( NULL == user)
	{
	    user = (user_ctx_t *)malloc(sizeof(user_ctx_t));
	    if ( NULL == user)
        {
			printf("NULL: failed to allocate memory\n");
            return;
        }
	    memset(user,0,sizeof(user_ctx_t));
	}
	user->server = (server_info_t *)malloc(sizeof(server_info_t));
	user->server->ip = inet_addr("192.168.0.2");
	user->server->port = 11111;

	socket_init(user->server);
	
	if (0==( user->master_qid = task_mq_create(100,sizeof(ipc_header_t),0,"user_master")))
	{
		printf("failed to create mq :%s\n",__FUNCTION__)
		return;
	}
	if ( 0 != task_spawn("master_task",25,600000,user_master_task,NULL))
	{
		printf("Failed to create user master task\n");
		return;
	}
	
	init_usr_msg_data_path(user);




}
