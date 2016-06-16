#ifndef _DECODER_API_H
#define _DECODER_API_H

#include "common.h"

typedef int (*Encoder_wrapper)(void *, int *);
typedef int (*Decoder_wrapper)();



typedef struct msg_pdu_handlers_
{
	int msg_type;
	Encoder_wrapper efunc;
	Decoder_wrapper dfunc;
}msg_pdu_handler_t;

int Request_E_Func(void *,int *);
int Request_D_func();

int Notification_Req_E(void *,int *);
int Notification_Req_D();

int Response_E_func(void*,int*);
int Response_D_Func();

int Notification_Res_E(void *,int *);
int Notification_Res_D();

int Termination_Req_E(void *,int *);
int Termination_Req_D();

int Termination_Res_E(void *, int *);
int Termination_Res_D(void *,int *);

int Decode_MSG(void *,int *,int *,void **);
int Encode_MSG(void *,unsigned char *,int *,int );





#endif
