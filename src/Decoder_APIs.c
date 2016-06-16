#include "Decoders_APIs.h"
#include "message.h"
/*
static msg_pdu_handler_t PDU_APIs[] = 
{	REQUEST,
	Request_E_Func,
	Request_D_Func,
},
{
	RESPONSE,
	Response_E_Func,
	Response_D_Func,
},
{
	NOTIFICATION_REQ,
	Notification_Req_E,
	Notification_Req_D,
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
	TERMINATION_RES
	Termination_Res_E,
	Termination_Res_D
};


int Request_E_Func()
{
	return 0;
}
int Request_D_Func()
{
	return 0;
}
int Response_E_Func()
{
	return 0;
}
int Response_D_Func()
{
	return 0;
}
int Notification_Req_E()
{
	return 0;
}
int Notification_Req_D()
{
	return 0;
}
int Notification_Res_E()
{
	return 0;
}
int Notification_Res_D()
{
	return 0;
}
int Termination_Req_E()
{
	return 0;
}
int Termination_Req_D()
{
	return 0;
}
int Termination_Res_E()
{
	return 0;
}
int Termination_Res_D()
{
	return 0;
}

*/
int Decode_MSG(void *data, int *len, int *msg_type, void **pvalue)
{
	return 0;
}

int Encode_MSG(void *msg, unsigned char *buff,int *len,int mt )
{
	return 0;
}

