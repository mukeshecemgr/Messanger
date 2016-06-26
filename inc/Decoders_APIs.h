#ifndef _DECODER_API_H
#define _DECODER_API_H

#include "common.h"

typedef int (*Encoder_wrapper)(void *,unsigned char *, int *);
typedef int (*Decoder_wrapper)(unsigned char *, void **,int);



typedef struct msg_pdu_handlers_
{
	int msg_type;
	Encoder_wrapper efunc;
	Decoder_wrapper dfunc;
}msg_pdu_handler_t;

typedef enum req_
{
  NAME = 101,
  AGE,
  SEX,
  MOB,
  LOCATION,
  CONTACTS,
  ONLINE_COUNT,
  DATA_PORT
}req_e;


int Request_E_Func(void *,unsigned char *,int *);
int Request_D_Func(unsigned char *, void **,int );

int Notification_Req_E(void *,unsigned char *,int *);
int Notification_Req_D(unsigned char *, void **,int );

int Response_E_Func(void *,unsigned char *,int *);
int Response_D_Func(unsigned char *, void **,int );

int Notification_Res_E(void *,unsigned char *,int *);
int Notification_Res_D(unsigned char *, void **,int );

int Termination_Req_E(void *,unsigned char *,int *);
int Termination_Req_D(unsigned char *, void **,int );

int Termination_Res_E(void *,unsigned char *,int *);
int Termination_Res_D(unsigned char *, void **,int );

int Decode_MSG(unsigned char *,int *,int *,void **);
int Encode_MSG(void *,unsigned char *,int *,int );





#endif
