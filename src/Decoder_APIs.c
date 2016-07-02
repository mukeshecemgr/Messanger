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
	NOTIFICATION,
	Notification_Req_E,
	Notification_Req_D
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

  int i_len = 0;
  int idx = 0, name_len = 0,mob_len = 0,location_len = 0, contact_len = 0;
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
  buff[i_len++] = mob_len;
  memcpy(buff+i_len,req->mob,mob_len);
  i_len += mob_len;

  buff[i_len++] = LOCATION;
  location_len = strlen(req->location)+1;
  buff[i_len++] = location_len;
  memcpy(buff+i_len,req->location,location_len);
  i_len += location_len;

  buff[i_len++] = CONTACTS;
  buff[i_len++] = req->contact_count;
  for ( idx = 0; idx < req->contact_count; idx++)
  {
      contact_len = strlen(req->contacts[idx].c_name_id)+1;
      buff[i_len++] = contact_len;
      memcpy(buff+i_len, req->contacts[idx].c_name_id, contact_len);
      i_len += contact_len;
      buff[i_len++] = req->contacts[idx].c_sex;

  }
  *len = i_len;
  return 0;
}
int Request_D_Func(unsigned char *data, void **pvalue,int len)
{
  int i_len = 0;
  int ie_type = 0, idx = 0;
  int name_len = 0,mob_len = 0, location_len = 0, contacts_count = 0, contact_len = 0;
  user_req_msg_t *req = (user_req_msg_t *)malloc(sizeof(user_req_msg_t));

  while(len > i_len)
  {
      ie_type = data[i_len++];
      switch(ie_type)
      {
          case NAME:
            name_len = data[i_len++];
            memcpy(req->name,data+i_len,name_len);
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
          case CONTACTS:
            contacts_count = data[i_len++];
            req->contact_count = contacts_count;
            for(idx = 0; idx < contacts_count; idx++)
            {
                contact_len = data[i_len++];
                memcpy(req->contacts[idx].c_name_id,data+i_len , contact_len);
                i_len += contact_len;
                req->contacts[idx].c_sex = data[i_len++];

            }
            break;
          default:
            printf("[%d][%s]Wrong IE type received : %d\n",__LINE__,__FUNCTION__,ie_type);

      }


  }
  *pvalue = req;
  return 0;
}
int Response_E_Func(void *data,unsigned char *buff,int *len)
{
    int i_len = 0;
    int name_len = 0;
    user_res_msg_t *res = (user_res_msg_t *)data;
    buff[i_len++] = NAME;
    name_len = strlen(res->name)+1;
    buff[i_len++] = name_len;
    memcpy(buff+i_len,res->name,name_len);
    i_len += name_len;
    buff[i_len++] = DATA_PORT;
    buff[i_len++] = res->data_port;

    *len = i_len;


    return 0;
}
int Response_D_Func(unsigned char *data, void **pvalue,int len)
{
    int i_len = 0;
    int ie_type = 0;
    int name_len = 0;
    user_res_msg_t *res = (user_res_msg_t *)malloc(sizeof(user_res_msg_t));

    while(len > i_len)
    {
        ie_type = data[i_len++];
        switch(ie_type)
        {
            case NAME:
              name_len = data[i_len++];
              memcpy(res->name,data+i_len,name_len);
              i_len += name_len;
              break;
            case DATA_PORT:
              res->data_port = data[i_len++];
              break;
            default:
              printf("[%d][%s]Wrong IE type received : %d\n",__LINE__,__FUNCTION__,ie_type);

        }


    }
    *pvalue = res;
    return 0;
}
int Notification_Req_E(void *data,unsigned char *buff,int *len)
{
  int i_len = 0;
  int idx = 0, user_count = 0, contact_len = 0;
  notification_t *notify = (notification_t *)data;
  buff[i_len++] = CONTACTS;
  user_count = notify->online_user_cnt;
  buff[i_len++] = user_count;
  for(idx = 0; idx < user_count ; idx++) {
      contact_len = strlen(notify->online_user[idx].online_uid) + 1;
      buff[i_len++] = contact_len;
      memcpy(buff+i_len,notify->online_user[idx].online_uid,contact_len );
      i_len += contact_len;
      buff[i_len++] = notify->online_user[idx].status;
  }

  *len = i_len;

  return 0;
}
int Notification_Req_D(unsigned char *data, void **pvalue,int len)
{
   int i_len = 0;
   int ie_type = 0;
   int contact_len = 0, count = 0, idx = 0;
   notification_t *notify = (notification_t *)malloc(sizeof(notification_t));

   while(len > i_len) {
       ie_type = data[i_len++];
       switch(ie_type)
       {
         case CONTACTS:
         count  = data[i_len++];
         notify->online_user_cnt = count;
         for(idx = 0; idx < count ; idx++) {
             contact_len = data[i_len++];
             memcpy(notify->online_user[idx].online_uid,data+i_len , contact_len);
             i_len += contact_len;
             notify->online_user[idx].status = data[i_len++];

         }
         break;
       default :
         printf("[%d][%s]Wrong IE type received : %d\n",__LINE__,__FUNCTION__,ie_type);
       }
   }
   *pvalue = notify;
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

  data += 2;
  msg_len -= 2;
  status = PDU_APIs[msg_type].dfunc(data,pvalue,msg_len);
  *mt = msg_type;
  return 0;
}

int Encode_MSG(void *msg, unsigned char *buff,int *len,int mt)
{

  int status = 0,msg_type = 0,msg_len = 0;
  if ( NULL == msg && NULL == buff)
  {
    printf("[%d][%s]NULL msg or buff received \n",__LINE__,__FUNCTION__);
    return -1;
  }

  status = PDU_APIs[mt].efunc(msg,buff+2,len);
  msg_type = mt;
  *len += 2;
  msg_len = *len;
  buff[0] = msg_type;
  buff[1] = msg_len;


  return status;
}

