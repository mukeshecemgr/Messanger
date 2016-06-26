#include "common.h"
#include "users.h"
#include "message.h"
#include "utils.h"

user_ctx_t *user = NULL;
extern unsigned char uname[30];
unsigned char mq_name[40] = {0};
void user_master_task(void *data)
{
	args_t *args = (args_t *)data;
	ipc_header_t ipc_msg;
	master_ipc_t *msg;
	int status;
	while(1)
	{
		status = task_mq_receive(mq_name,user->master_qid,
									(char *)&ipc_msg,
									sizeof(ipc_header_t),
									(unsigned int) -1);
		if ( status == 1)
			continue;
		msg = (master_ipc_t *)ipc_msg.data;
		switch(ipc_msg.type)
		{
			case RESPONSE:
			  user_handle_response_message(msg->data,msg->ctx);
			break;
			case NOTIFICATION:
			  user_handle_notification_msg(msg->data,msg->ctx);
			break;
			case TERMINATION_REQ:
			//	user_handle_chat_termination_notification(msg->data,msg->ctx);
			break;
			case TERMINATION_RES:
			//	user_notify_for_session_termination(msg->data,msg->ctx);
			break;
			default:
			printf("Fatel Error: Wrong message type received\n");
		}
	}
}

void fill_user_info(user_info_t *me)
{
	//printf("Enter personal Information\n");
//	printf("Name:");
//	scanf("%s",me->name);
//	printf("Age:");
//	scanf("%d",&me->age);
	//printf("Sex :");
//	scanf("%c",&me->sex);
	return;
}

void online_friend_task(void *data)
{
  online_users_t *ol_friends = NULL;
  slist_t *list = NULL;
  int counter=0, i = 0;
  printf("|\n----------------------------------------------------------\n");
  while (1) {
      if ( NULL == user->online) {
              continue;
      }
      for(i  = 0;i < counter+1 ; i++) {
          fputs("\033[A\033[2K",stdout);
      }
      list = user->online;
      counter = 0;
      while(list != NULL) {
        ol_friends = (online_users_t*)list->node;
        printf("|%d. %s\t--- %s\n",counter+1,ol_friends->online_uid,
                                    (ol_friends->status == 1)?"Online":"Offline");
        counter++;
        list = list->next;
      }
      printf("|----------------------------------------------------------\n");
      rewind(stdout);
      sleep(2);


  }
}
void user_init()
{

        args_t args;
	if ( NULL == user)
	{
	    user = (user_ctx_t *)malloc(sizeof(user_ctx_t));
	    if ( NULL == user)
        {
			printf("NULL: failed to allocate memory\n");
            exit(1);
        }
	    memset(user,0,sizeof(user_ctx_t));
	}
	user->me = (user_info_t *)malloc(sizeof(user_info_t));
	memset(user->me,0,sizeof(user_info_t));

	fill_user_info(user->me);

	memset(&args,0,sizeof(args_t));

        strcpy(mq_name,USER_MASTER_Q);
        strcat(mq_name,uname);
	user->master_qid = task_mq_create(100,sizeof(ipc_header_t),0,mq_name);
	if (0 > user->master_qid )
	{
		printf("failed to create mq :%s\n",__FUNCTION__);
		exit(1);
	}
	if ( 0 != task_spawn("master_task",25,1600000,(PFN)user_master_task,&args))
	{
		printf("Failed to create user master task\n");
		exit(1);
	}

	/**
	 * Initialize control path and send request message
	 */
	
	user->ctrl_conn = (connection_info_t *)malloc(sizeof(connection_info_t));
	memset(user->ctrl_conn,0,sizeof(connection_info_t));

	init_ctrl_path(user->ctrl_conn);
	
	if ( 0 != prepare_and_send_req_msg(user->ctrl_conn,user->me))
	{
		printf("Failed to prepare request message\n");
		return;
	}

	if ( 0 != task_spawn("Online_friend_task",25,1600000,(PFN)online_friend_task,&args))
        {
                printf("Failed to create user master task\n");
                exit(1);
        }
	init_usr_msg_data_path(user->data_conn);




}

void init_ctrl_path(connection_info_t *ctrl)
{
	args_t args;
	strcpy(ctrl->ip,"127.0.0.1");
	ctrl->port = 11111;
	ctrl->msg_cb = ctrl_recv_cb;

	if( 0 > (ctrl->sd = socket(AF_INET,SOCK_DGRAM,0)) )
	{
		perror("socket()");
		exit(1);
	}

	ctrl->srvr.sin_family = AF_INET;
	ctrl->srvr.sin_port = ctrl->port;
	ctrl->srvr.sin_addr.s_addr = inet_addr(ctrl->ip);

	args.arg1 =(int) ctrl;
/*
if (0==( ctrl->rx_qid = task_mq_create(100,sizeof(ipc_header_t),0,"ctrl_rx")))
	{
		printf("failed to create mq :%s\n",__FUNCTION__);
		exit(1);
	}
	*/
	if ( 0 != task_spawn("UE_rx_task",25,600000,(PFN)user_ctrl_rx_task,&args))
	{
		printf("Failed to create user master task\n");
		exit(1);
	}

	
	
	
}
void init_usr_msg_data_path()
{
	
	online_userinfo_t *they = NULL;
}

