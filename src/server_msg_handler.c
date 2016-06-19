#include "message.h"
#include "server.h"
#include "server_msg_handler.h"

int server_handle_user_req_msg(void *data, void *ctx)
{

  printf("Request Message received\n");
  user_req_msg_t *req = (user_req_msg_t *)data;
  printf("Name : %s\n",req->name);
  printf("Age : %d\n",req->age);
  printf("Sex : %c\n",req->sex);
  printf("Mobile : %s\n",req->mob);
  printf("Location : %s\n",req->location);

	return 0;
}

int server_handle_user_termination_msg(void *data,void *ctx)
{
	return 0;
}

