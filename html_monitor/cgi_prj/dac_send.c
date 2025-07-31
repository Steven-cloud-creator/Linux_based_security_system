#include "data_global.h"
#include "cgic.h"
#define N 8

int cgiMain()
{
    key_t msg_key;
    char buf[5];
    int msgid;
    struct msg msg_buf;
    memset(&msg_buf,0,sizeof(msg_buf));
    // 检查表单数据获取是否成功
    if (cgiFormString("dac_value", buf, sizeof(buf)) != cgiFormSuccess) {
        cgiHeaderContentType("text/html\n\n");
        fprintf(cgiOut, "<HTML><HEAD>\n");
        fprintf(cgiOut, "<TITLE>Error</TITLE></HEAD>\n");
        fprintf(cgiOut, "<BODY>");
        fprintf(cgiOut, "<H2>Failed to get form data</H2>");
        fprintf(cgiOut, "</BODY>\n");
        fprintf(cgiOut, "</HTML>\n");
        return 1;
    }

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
    msg_buf.text[0] = atoi(buf);
    
    if (msgsnd(msgid, &msg_buf, sizeof(msg_buf) - sizeof(long), 0) < 0) {
        perror("msgsnd");
        cgiHeaderContentType("text/html\n\n");
        fprintf(cgiOut, "<HTML><HEAD>\n");
        fprintf(cgiOut, "<TITLE>Error</TITLE></HEAD>\n");
        fprintf(cgiOut, "<BODY>");
        fprintf(cgiOut, "<H2>Failed to send message</H2>");
        fprintf(cgiOut, "</BODY>\n");
        fprintf(cgiOut, "</HTML>\n");
        return 1;
    }

    cgiHeaderContentType("text/html\n\n"); 
    fprintf(cgiOut, "<HTML><HEAD>\n"); 
    fprintf(cgiOut, "<TITLE>My CGI</TITLE></HEAD>\n"); 
    fprintf(cgiOut, "<BODY>"); 

    fprintf(cgiOut, "<H2>send sucess</H2>");

    fprintf(cgiOut, "</BODY>\n"); 
    fprintf(cgiOut, "</HTML>\n"); 
    return 0;
}