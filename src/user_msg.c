#include "common.h"
#include "users.h"
#include "message.h"




extern user_ctx_t *user;
extern unsigned char uname[30];
extern unsigned char mq_name[40];

static int compare_user_id(void *list_elm, void *item)
{
  //  printf("%s == %s\n",((user_conn_details_t *)list_elm)->user_ctx->name,(unsigned char *)item) ;
    if ( ! (strcmp(((online_users_t *)list_elm)->online_uid,(unsigned char *)item) ) ) {
        return 0;
    }
    return -1;
}

int user_handle_response_message(void *msg,void *ctx)
{
    user_res_msg_t *res = (user_res_msg_t *)msg;
}
int user_handle_notification_msg(void *msg,void *ctx)
{
  online_users_t *online_friend = NULL;
  notification_t *notify = (notification_t *)msg;
  int idx = 0;


  for(idx = 0; idx < notify->online_user_cnt ; idx++) {

      if (NULL != user->online) {
          online_friend = (online_users_t *)slist_find(user->online,compare_user_id,notify->online_user[idx].online_uid);
      }
      if ( NULL != online_friend) {
          online_friend->status = notify->online_user[idx].status;
      } else {
          online_friend = (online_users_t *)malloc(sizeof(online_users_t));
          memset(online_friend,0,sizeof(online_users_t));
          strcpy(online_friend->online_uid,notify->online_user[idx].online_uid);
          online_friend->status = notify->online_user[idx].status;

          slist_add(&user->online,(void *)online_friend);
      }
 //     printf("%s : %d\n",online_friend->online_uid,online_friend->status);
  }

 // start_chatting(online_friend);

}

int start_chatting(user_info_t *user_info,char *friend)
{


}

int user_handle_chat_termination_notification(void *data,void *ctx)
{
}
int user_notify_for_session_termination(void *data,void *ctx)
{
	
}

int get_contacts(contact_info_t *temp_contact[],char *contact_dir,int *count)
{
  FILE *fd;
  int idx = 0;
  fd = fopen(contact_dir, "r");
  if ( NULL == fd){
      printf("Failed to open File : %s\n",contact_dir);
      return -1;
  } else {
      while(!feof(fd)) {
          temp_contact[idx] = (contact_info_t *)malloc(sizeof(contact_info_t));
          fscanf(fd,"%s\n",temp_contact[idx]->c_name_id);
          fscanf(fd,"%c\n",&temp_contact[idx++]->c_sex);
      }
      fclose(fd);
  }
  *count = idx;
  return 0;
}

int prepare_and_send_req_msg(connection_info_t *ctrl_conn,user_info_t *me)
{

    char contact_dir[100] = {0};
    contact_info_t *temp_contact[1024] = {0};
	unsigned char buff[1024] = {0};
	int len  = 0;
	user_req_msg_t *req = NULL;
	int idx = 0;
	int contact_count= 0;

	strcpy(contact_dir,CONTACT_FILE);
	strcat(contact_dir,uname);
	if ( 0 != get_contacts(temp_contact,contact_dir,&contact_count) ){
	    printf("Failed to get user contact information\n");
	}
	req = (user_req_msg_t *)malloc(sizeof(user_req_msg_t));
	memset(req,0,sizeof(user_req_msg_t));
	
	memcpy(req->name,me->name,strlen(me->name)+1);
	req->age = me->age;
	req->sex = me->sex;
	memcpy(req->mob,me->mobile_num,strlen(me->mobile_num)+1);
	memcpy(req->location,me->location,strlen(me->location)+1);
	//printf("Contact Length : %d\n",contact_len);
	for(idx = 0; idx < contact_count;idx++)
        {
	    memcpy(req->contacts[idx].c_name_id,temp_contact[idx]->c_name_id,
	                              strlen(temp_contact[idx]->c_name_id)+1);
	    req->contacts[idx].c_sex = temp_contact[idx]->c_sex;
	    req->contact_count++;

        }
	memset(buff,'\0',1024);
	if ( 0 != Encode_MSG((void *)req,buff,&len,REQUEST))
	{
		printf("Failed to encode REQUEST message\n");
		return 1;
	}
	if ( 0 > (sendto(ctrl_conn->sd,buff,len,0,(struct sockaddr *)&ctrl_conn->srvr,
					sizeof(struct sockaddr))) )
	{
		printf("[%d][%s]Failed : to send request message to server\n",__LINE__,__FUNCTION__);
		return 1;
	}
	return 0;

}

/**
 * Receive message from control socket and decode here itself, Then post to master MQ
 */
int ctrl_recv_cb(int len,unsigned char *data, void *ctx)
{
        //  printf("User Received message \n");

          ipc_header_t ipc;
          master_ipc_t *msg = (master_ipc_t *)malloc(sizeof(master_ipc_t));
          int mt = 0;
          void *pvalue = NULL;
          if ( 0 != Decode_MSG(data,&len,&mt,&pvalue))
          {
                  printf("[%d][%s]failed to decode message\n",__LINE__,__FUNCTION__);
                  return;
          }
       //   printf("Pvalue : %p\n",pvalue);
          msg->data = pvalue;
          msg->ctx = ctx;

          /*Send to master task, Where all control message will be
          * processes */
          ipc.type = mt;
          ipc.len = sizeof(ipc_header_t);
          ipc.data = msg;
          if ( 0 != task_mq_send(mq_name,user->master_qid,
                                          (char *)&ipc,sizeof(ipc_header_t),0))
          {
                  printf("[%d][%s]Failed to post into mq\n",__LINE__,__FUNCTION__);
                  return;
          }

	return 0;
}

