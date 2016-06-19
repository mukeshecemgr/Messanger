#include "Decoders_APIs.h"
#include "message.h"

static msg_pdu_handler_t PDU_APIs[] = {
{	REQUEST,
	Request_E_Func,
	Request_D_Func
},
{
	RESPONSE,
	Response_E_Func,
	Response_D_Func
},
{
	NOTIFICATION_REQ,
	Notification_Req_E,
	Notification_Req_D
},
{
	NOTIFICATION_RES,
	Notification_Res_E,
	Notification_Res_D
},
{
	TERMINATION_REQ,
	Termination_Req_E,
	Termination_Req_D
},
{
	TERMINATION_RES,
	Termination_Res_E,
	Termination_Res_D
} };


int Request_E_Func(void *data,unsigned char *buff,int *len)
{
  printf("Encoding ------------->\n");
  int i_len = 0;
  int name_len = 0,mob_len = 0,location_len = 0;
  user_req_msg_t *req = (user_req_msg_t *)data;

  buff[i_len++] = NAME;
  name_len = strlen(req->name)+1;
  buff[i_len++] = name_len;
  memcpy(buff+i_len, req->name,name_len);

  i_len += name_len;
  buff[i_len++] = AGE;
  buff[i_len++] = req->age;
  buff[i_len++] = SEX;
  buff[i_len++] = req->sex;
  buff[i_len++] = MOB;
  mob_len = strlen(req->mob)+1;
  memcpy(buff+i_len,req->mob,mob_len);
//  printf("Emob :%s\n",req->mob);
  i_len += mob_len;
  location_len = strlen(req->location)+1;
  memcpy(buff+i_len,req->location,location_len);
//  printf("Elocation :%s\n",req->location);
  i_len += location_len;

  *len = i_len;

  return 0;
}
int Request_D_Func(unsigned char *data, void **pvalue,int len)
{
  printf("Len received in Decode : %d\n",len);
  int i_len = 0;
  int ie_type = 0;
  int name_len = 0,mob_len = 0, location_len = 0;
  user_req_msg_t *req = (user_req_msg_t *)malloc(sizeof(user_req_msg_t));

  while(len > i_len)
  {
      ie_type = data[i_len++];
      switch(ie_type)
      {
          case NAME:
            name_len = data[i_len++];
            memcpy(req->name,data+i_len,name_len);
            printf("DName : %s\n",req->name);
            i_len += name_len;
            break;
          case AGE:
            req->age = data[i_len++];
            break;
          case SEX:
            req->sex = data[i_len++];
            break;
          case MOB:
            mob_len = data[i_len++];
            memcpy(req->mob,data+i_len,mob_len);
            i_len += mob_len;
            break;
          case LOCATION:
            location_len = data[i_len++];
            memcpy(req->location,data+i_len,location_len);
            i_len += location_len;
            break;
          default:
            printf("[%d][%s]Wrong IE type received\n",__LINE__,__FUNCTION__);

      }


  }
  *pvalue = req;
  return 0;
}
int Response_E_Func(void *data,unsigned char *buff,int *len)
{
	return 0;
}
int Response_D_Func(unsigned char *data, void **pvalue,int len)
{
	return 0;
}
int Notification_Req_E(void *data,unsigned char *buff,int *len)
{
	return 0;
}
int Notification_Req_D(unsigned char *data, void **pvalue,int len)
{
	return 0;
}
int Notification_Res_E(void *data,unsigned char *buff,int *len)
{
	return 0;
}
int Notification_Res_D(unsigned char *data, void **pvalue,int len)
{
	return 0;
}
int Termination_Req_E(void *data,unsigned char *buff,int *len)
{
	return 0;
}
int Termination_Req_D(unsigned char *data, void **pvalue,int len)
{
	return 0;
}
int Termination_Res_E(void *data,unsigned char *buff,int *len)
{
	return 0;
}
int Termination_Res_D(unsigned char *data, void **pvalue,int len)
{
	return 0;
}


int Decode_MSG(unsigned char *data, int *len, int *mt, void **pvalue)
{
  int status = 0;
  int msg_type = data[0];
  int msg_len = data[1];

 // printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>.. mt : %d\tlen : %d\n",msg_type,msg_len);
  data += 2;
  msg_len += 2;
  status = PDU_APIs[msg_type].dfunc(data,pvalue,msg_len);
  *mt = msg_type;
  return 0;
}

int Encode_MSG(void *msg, unsigned char *buff,int *len,int mt)
{

  int status = 0,msg_type = 0,msg_len = 0;
  if ( NULL == msg && NULL == buff)
  {
    printf("[%d][%s]NULL msg or buff received\n",__LINE__,__FUNCTION__);
    return -1;
  }
;

  status = PDU_APIs[mt].efunc(msg,buff+2,len);
  msg_type = mt;
  *len += 2;
  msg_len = *len;
  buff[0] = msg_type;
  buff[1] = msg_len;



  printf("Ecode Len : %d\n",msg_len);
  return status;
}

