#ifndef _UTILS_H
#define _UTILS_H

#include <sched.h>
#include "common.h"
#include <pthread.h>
#include "unistd.h"
#define __USE_XOPEN2K 1 
#include <mqueue.h>
#undef __USE_XOPEN2K

#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#define         MSG_PRI_NORMAL   0 
#define         NO_WAIT          0
#define         WAIT_FOREVER    -1
#define         BLOCK_MODE       0
#define         NON_BLOCK_MODE   1
#define         MODEMDEVICE      "/dev/ttyS0"
#define         MAX_READ_BUFF    256

typedef int
(*slist_find_compare_func) (void *list_elem,
                               void *user_data);

typedef void *(*PFN)(void *);


void *slist_find(slist_t *, slist_find_compare_func ,void *);
void slist_add(slist_t **, void *);
int task_spawn(char *, int , int, void *(*function)(void *),void *);
int task_mq_create(int,int, int, char *);
int task_mq_send(unsigned char *,int , char *, int, int);
int task_mq_receive(unsigned char *,int, char *, unsigned int, int);


#endif
