#include "data_global.h"

extern int msgid;
extern key_t msg_key;


extern unsigned int cmd_dac;

struct msg msgbuf;

void *pthread_client(void *arg)
{
    if((msg_key = ftok("/tmp",'g')) < 0)
    {
        perror("ftok failed .\n");
        exit(-1);
    }

    msgid = msgget(msg_key,IPC_CREAT|IPC_EXCL|0666);
    if(msgid == -1)
    {
        if(errno == EEXIST)
            msgid = msgget(msg_key,0777);
        else
        {
            perror("fail to msgget");
            exit(1);
        }
    }
    printf("pthread_client\n");

    while(1)
    {
        bzero(&msgbuf,sizeof(msgbuf));
        printf("wait form client request...\n");
        msgrcv (msgid, &msgbuf, sizeof (msgbuf) - sizeof (long), 1L, 0);
		printf ("Get %ldL msg\n", msgbuf.msgtype);
		printf ("text[0] = %d\n", msgbuf.text[0]);

        switch(msgbuf.msgtype)
        {
            case 1L:
                pthread_mutex_lock(&mutex_dac);
                printf("hello dac\n");
                cmd_dac = msgbuf.text[0];
                pthread_mutex_unlock(&mutex_dac);
                pthread_cond_signal(&cond_dac);
                break;
            default:
                break;
        }
    }
}