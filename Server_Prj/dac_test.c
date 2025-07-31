#include "data_global.h"

#define N 8


int main()
{
    key_t msg_key;
    char buf[N];
    int msgid;
    struct msg msg_buf;
    memset(&msg_buf,0,sizeof(msg_buf));

    if((msg_key = ftok("/tmp",'g'))<0)
    {
        perror("ftok");
        exit(1);
    }

    if((msgid = msgget(msg_key,0666)) < 0)
    {
        perror("msgget");
        exit(1);
    }

    bzero(msg_buf.text,sizeof(msg_buf.text));

    msg_buf.type = 1L;
    msg_buf.msgtype = 1L;
    msg_buf.text[0] = 500;
    msgsnd(msgid,&msg_buf,sizeof(msg_buf)-sizeof(long),0);
    return 0;
}