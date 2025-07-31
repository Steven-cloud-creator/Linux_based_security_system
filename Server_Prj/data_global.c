#include "data_global.h"
/***************************************
    全局的互斥体声明
    全局的条件锁声明
    全局的id和key信息声明
    全局的消息队列发送函数声明
****************************************/

int send_msg_queue(long type,unsigned int text)
{
    struct msg msgbuf;
    msgbuf.type = 1L;
    msgbuf.msgtype = type;
    msgbuf.text[0] = text;

    if(msgsnd(msgid,&msgbuf,sizeof(msgbuf) - sizeof(long),0) == -1)
    {
        perror("fail to msgsnd");
        exit(1);
    }

    return 0;
}



