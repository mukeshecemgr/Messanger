#include "common.h"
#include <signal.h>

unsigned char uname[30] = {0};
int signal_handler()
{
	/* Write all thread, socket and db de-initialize functions*/
	printf("Successfully exit ...\n");
}

int main(int argc, char *argv[])
{
//	signal(SIGINT,signal_handler);
//	signal(SIGQUIT,signal_handler);
	int opt;

	if ( argc < 2) {
	    printf("Please pass args as username\n");
	    return 0;
	}
        printf("|-----------------------------\n");
        printf("|1.Server\n|2.User\n|3.Press ctrl+c to close :");
        scanf("%d",&opt);
        switch(opt)
        {
                case SERVER:
                        server_init();
                        break;
                case USER:
                        strcpy(uname,argv[1]);
                        user_init();
                        break;
                case ADMIN:
//					admin_task();
                        break;
                default:
                        printf("Wrong option selected");
        }

	while(1)
	{
		sleep(1000);
	}
}
