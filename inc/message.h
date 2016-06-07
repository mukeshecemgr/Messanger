#include "common.h"

typedef struct user_req_msg_
{
	char name[100];//M
	int age;//O
	char sex;//M
	char mob[11];//O
	char location[30];//M
}user_req_msg_t;

typedef struct user_res_msg_
{
	char name[100]//M
	int data_port;
}user_res_msg_t;
