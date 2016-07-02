#include "message.h"
#include "server.h"
#include "server_msg_handler.h"
#include "utils.h"

extern server_ctx_t *srvr_ctx;
int data_port = 22222;

static int compare_user_id(void *list_elm, void *item)
{
  //  printf("%s == %s\n",((user_conn_details_t *)list_elm)->user_ctx->name,(unsigned char *)item) ;
    if ( ! (strcmp(((user_conn_details_t *)list_elm)->user_ctx->name,(unsigned char *)item) ) ) {
        return 0;
    }
    return -1;
}

void per_user_notification(void *data)
{
    unsigned char *name = (unsigned char *)data;
    unsigned char buff[1024] = {0};
    int len  = 0;
    int idx = 0;
    notification_t *notify = NULL;
    user_conn_details_t *user_conn = NULL, *online_usr_conn = NULL;
    notify = (notification_t *)malloc(sizeof(notification_t));
    memset(notify,0,sizeof(notification_t));
    while(1) {
        user_conn = ( user_conn_details_t *)slist_find(srvr_ctx->users_connected,compare_user_id,name);
        if ( NULL == user_conn) {
            printf("NULL List, Fatal error\n");
            return ;
        }
        for( idx = 0; idx < user_conn->user_ctx->contact_count;idx++) {

            online_usr_conn = ( user_conn_details_t *)slist_find(srvr_ctx->users_connected,
                                        compare_user_id,user_conn->user_ctx->contacts[idx].c_name_id);
            if ( NULL == online_usr_conn) {
                //Deal with this later
                strcpy(notify->online_user[idx].online_uid,user_conn->user_ctx->contacts[idx].c_name_id);
                notify->online_user[idx].status = 3;
            } else {
                strcpy(notify->online_user[idx].online_uid,user_conn->user_ctx->contacts[idx].c_name_id);
                if ( online_usr_conn->reg_state == TRUE ) {
                    notify->online_user[idx].status = 1;
                } else {
                    notify->online_user[idx].status = 2;
                }
            }
        }
        notify->online_user_cnt = user_conn->user_ctx->contact_count;
        if ( 0 != Encode_MSG((void *)notify,buff,&len,NOTIFICATION))
        {
            printf("Failed to encode RESPONSE message\n");
            return ;
        }
        if ( 0 > (sendto(srvr_ctx->serv_conn->sd,buff,len,0,(struct sockaddr *)user_conn->ctx,
                                        sizeof(struct sockaddr))) )
        {
            printf("Failed : to send response message to server\n");
            return ;
        }
        sleep(5);

    }




}

int prepare_send_res_message(user_req_msg_t *req, void *ctx)
{
    unsigned char buff[1024] = {0};
    int len  = 0;
    user_res_msg_t *res = NULL;
    res = (user_res_msg_t *)malloc(sizeof(user_res_msg_t));
    memset(res,0,sizeof(user_res_msg_t));
    strcpy(res->name,req->name);
    res->data_port = data_port++;

    if ( 0 != Encode_MSG((void *)res,buff,&len,RESPONSE))
    {
        printf("Failed to encode RESPONSE message\n");
        return 1;
    }
    if ( 0 > (sendto(srvr_ctx->serv_conn->sd,buff,len,0,(struct sockaddr *)ctx,
                                    sizeof(struct sockaddr))) )
    {
        printf("Failed : to send response message to server\n");
        return 1;
    }
}

/******************************************
 * Handle req message and send response
 */

int server_handle_user_req_msg(void *data, void *ctx)
{
    struct sockaddr_in *clnt = (struct sockaddr_in *)ctx;
    user_conn_details_t *user_conn = NULL;
    user_req_msg_t *req = (user_req_msg_t *)data;
    int idx = 0;
    unsigned char uname[30] = {0};

    user_conn = (user_conn_details_t *)malloc(sizeof(user_conn_details_t));
    memset(user_conn,0,sizeof(user_conn_details_t));
    user_conn->user_ctx = (user_req_msg_t *)malloc(sizeof(user_req_msg_t));
    memset(user_conn->user_ctx,0,sizeof(user_details_t));
    strcpy(user_conn->user_ctx->name,req->name);
    user_conn->user_ctx->sex = req->sex;
    user_conn->user_ctx->age = (int)req->age;
    strcpy(user_conn->user_ctx->mob,req->mob);
    strcpy(user_conn->user_ctx->location,req->location);
    user_conn->user_ctx->contact_count = req->contact_count;

    for (idx = 0; idx < user_conn->user_ctx->contact_count; idx++){
        strcpy(user_conn->user_ctx->contacts[idx].c_name_id,req->contacts[idx].c_name_id);
        user_conn->user_ctx->contacts[idx].c_sex = req->contacts[idx].c_sex;

    }

    memcpy((char *)user_conn->ctx,(char *)clnt,sizeof(struct sockaddr_in));
    user_conn->reg_state = TRUE;

    strcpy(uname,req->name);
    /**
     * Adding to list
     */
    slist_add(&srvr_ctx->users_connected,(void *)user_conn);

    if ( 0 == prepare_send_res_message(req,ctx))
    {
        printf("Failed to send response message\n");
        return -1;
    }

    if ( 0 != task_spawn("Notification_task",25,1600000,(PFN)per_user_notification,(void *)req->name))
    {
            printf("Failed to create Per Usr Notification task\n");
            exit(1);
    }
    /*
    if ( 0 == prepare_send_notification(req->name) ){
        printf("Failed to send response message\n");
        return -1;
    } */
    return 0;
}
/*****************************************
 * NOtification Message Prep and Sending
 */
int prepare_send_notification(unsigned char *name) {

  unsigned char buff[1024] = {0};
  int len  = 0;
  int idx = 0;
  notification_t *notify = NULL;
  user_conn_details_t *user_conn = NULL, *online_usr_conn = NULL;
  notify = (notification_t *)malloc(sizeof(notification_t));
  memset(notify,0,sizeof(notification_t));

  user_conn = ( user_conn_details_t *)slist_find(srvr_ctx->users_connected,compare_user_id,name);
  if ( NULL == user_conn) {
      printf("NULL List, Fatal error\n");
      return -1;
  }
  for( idx = 0; idx < user_conn->user_ctx->contact_count;idx++) {

      online_usr_conn = ( user_conn_details_t *)slist_find(srvr_ctx->users_connected,
                                  compare_user_id,user_conn->user_ctx->contacts[idx].c_name_id);
      if ( NULL == online_usr_conn) {
  //        printf("NoT Found : %s\n",user_conn->user_ctx->contacts[idx].c_name_id);
          //Deal with this later
          strcpy(notify->online_user[idx].online_uid,user_conn->user_ctx->contacts[idx].c_name_id);
          notify->online_user[idx].status = 3;
      } else {
      //    printf("Found : %s\n",user_conn->user_ctx->contacts[idx].c_name_id);
          strcpy(notify->online_user[idx].online_uid,user_conn->user_ctx->contacts[idx].c_name_id);
          if ( online_usr_conn->reg_state == TRUE ) {
              notify->online_user[idx].status = 1;
          } else {
              notify->online_user[idx].status = 2;
          }
      }
  }
  notify->online_user_cnt = user_conn->user_ctx->contact_count;
 // printf("User contacts count : %d\n",notify->online_user_cnt);
  if ( 0 != Encode_MSG((void *)notify,buff,&len,NOTIFICATION))
  {
      printf("Failed to encode RESPONSE message\n");
      return -1;
  }
  if ( 0 > (sendto(srvr_ctx->serv_conn->sd,buff,len,0,(struct sockaddr *)user_conn->ctx,
                                  sizeof(struct sockaddr))) )
  {
      printf("Failed : to send response message to server\n");
      return -1;
  }






}

int server_handle_user_termination_msg(void *data,void *ctx)
{
	return 0;
}

