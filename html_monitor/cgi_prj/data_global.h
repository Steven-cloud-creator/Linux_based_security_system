#ifndef __DATA_GLOBAL__H__
#define __DATA_GLOBAL__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <termios.h>
#include <syscall.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <linux/input.h>

/***************************************
    全局的宏定义
    全局的线程函数声明
    全局的设备节点声明
    全局的消息队列发送函数外部extern声明
    全局的消息队列传递的结构体信息声明
****************************************/
#define MONITOR_NUM 1
#define QUEUE_MSG_LEN  32
/***************************************
 * 全局的设备节点声明
***************************************/
#define DHT11_DEV "/dev/zh_dht11"
#define DAC_DEV "/dev/zh_dac"


unsigned int cmd_dac;

/***************************************
 * 全局的结构体声明
***************************************/
struct dht11_info{
    float humidity;      //湿度
    float temperature;   //温度
};

struct env_data{        //单个房间的环境数据
    struct dht11_info   dht11_info;
};

struct env_info{
    struct env_data monitor_no[MONITOR_NUM];
}__attribute__((aligned (1)));

pthread_mutex_t mutex_client;
pthread_mutex_t mutex_refresh;
pthread_mutex_t mutex_transfer;
pthread_mutex_t mutex_dac;
pthread_cond_t cond_client;
pthread_cond_t cond_refresh;
pthread_cond_t cond_transfer;
pthread_cond_t cond_dac;

struct env_info security_env_info;    //安防监控项目所有的环境信息

int semid;          //信号量集标识符
int shmid;          //共享内存标识符
int msgid;          //消息队列标识符

key_t msg_key;
key_t sem_key;      //信号量的键值
key_t shm_key;      //共享内存键值


/***************************************
 * 全局的外部线程函数声明
***************************************/

extern void *pthread_client(void *arg);               //客户端请求线程
extern void *pthread_refresh(void *arg);              //刷新共享内存数据线程
extern void *pthread_transfer(void *arg);             //接收传感器数据并解析
extern void *pthread_dac(void *arg);                  //控制DAC线程

//extern int send_msg_queue(long type,unsigned int text);
struct msg
{
    long type;                          //从消息队列接收消息时用于判断的消息类型
    long msgtype;                       //具体的消息类型
    unsigned int text[QUEUE_MSG_LEN];   //消息正文
};



#endif


