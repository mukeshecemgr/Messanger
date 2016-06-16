#include "common.h"
#include "utils.h"
void slist_add(slist_t **list, void *item)
{
	slist_t *new = NULL;

	new = (slist_t *)malloc(sizeof(slist_t));
	new->node = item;
	new->next = NULL;
	if ( *list == NULL )
	{
		*list = new;
	} else {
		new->next = *list;
		*list = new;
	}
}


int task_spawn(char *name, int vx_priority,int stack_size,
					void *(*function)(void*),void *args)
{
	pthread_attr_t thread_attributes;
    pthread_t thread_id;
	struct sched_param param;
    unsigned int priority;
	
	/* Initialise the thread attributes structure */
	if (0 != pthread_attr_init(&thread_attributes))
	{
		printf ("failed to initiase the thread attributes\n");
		return -1;
	}
	if (0 != pthread_attr_setdetachstate (&thread_attributes,
		                                 PTHREAD_CREATE_DETACHED))
	{
		printf ("failed to set detach state\n");
		return -1;
	}
	if (0 != pthread_attr_setschedpolicy (&thread_attributes, SCHED_FIFO))
	{
		printf ("failed to set schedule policy\n");
		return -1;
	}

	if (0 != pthread_attr_setstacksize (&thread_attributes, (size_t)stack_size))
	{
		printf ("%s task : failed to set the stack size\n", name );
	    return -1;
	}
	if (vx_priority < 99)
	{
		priority = 99 - vx_priority;
	} else {
		priority = 1;
	}
	param.sched_priority = priority;
    if (0 != pthread_attr_setschedparam (&thread_attributes, &param))
    {
		printf ("failed to set the priority\n");
		return -1;
	}
	if (0 != pthread_attr_setinheritsched (&thread_attributes,
                                    PTHREAD_EXPLICIT_SCHED))
    {
        printf  ("failed to set inherit\n");
		return -1;
	}
	if (0 != pthread_create (&thread_id, &thread_attributes, function, args))
	{
		printf ("%s task :failed to create a thread\n",name);
		return -1;
	}
	return 0;
}


int
task_mq_create (int max_msgs,
                        int max_len,
                        int options,
                        char * name)
{
    struct mq_attr queue_attributes;
    mqd_t mq_id;
    /* Access mode 0666 - user,group,owner*/
    int mode = 0666;//saravanan

    queue_attributes.mq_maxmsg = (long int)max_msgs;
    queue_attributes.mq_msgsize = (long int)max_len;
    queue_attributes.mq_flags = (long int)options;

    if(name == NULL)
    {
        printf ("%s", "msq_q name is NULL\n");
        return 0;
    }
    /*
    * unlink the queue to discard the previous messages
    */
    mq_unlink(name);

    if (BLOCK_MODE == options)
    {
        mq_id = mq_open (name, O_CREAT|O_RDWR, mode, &queue_attributes);
    }
    else
    {
        mq_id = mq_open (name, O_CREAT|O_RDWR|O_NONBLOCK, mode,
                                                            &queue_attributes);
    }
    if(-1 == mq_id)
    {
        printf ("error no %d with msq_q name %s\n", errno, name);
        return 0;
    }
    return (unsigned int)mq_id;
}


int
task_mq_send (unsigned char *mq_name,
             int mq_id,
             char *msg,
             int msg_len,
             int options)
{
    if (-1 == mq_send (mq_id, msg, (size_t)msg_len, options))
    {
        printf ("failed to send with error no %d message with name %s\n",
                                                            errno, mq_name);
        return -1;
    }
    return 0;
}


int
task_mq_receive (unsigned char *mq_name,
                int msgqid,
                char *msg,
                unsigned int msg_len,
                int timeout)
{
    struct timespec td;

	if(timeout == NO_WAIT || timeout == WAIT_FOREVER)
    {
        if (-1 == mq_receive ((mqd_t)msgqid, msg, (size_t)msg_len, NULL))
        {
			if (timeout != NO_WAIT)
			{
				printf ("failed to receive with err no %d a"
				              "message with name %s\n", errno, mq_name);
			}
            return -1;
        }
    }
    else
    {
        clock_gettime(CLOCK_REALTIME, &td);
        /*
             timeout in nano seconds
        */
        td.tv_nsec += timeout;
        if (-1 == mq_timedreceive ((mqd_t)msgqid, msg, (size_t)msg_len, NULL,
                                    (struct timespec *) &td))
        {
            return -1;
        }
    }
    return 0;
}




