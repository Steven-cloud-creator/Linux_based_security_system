#include "data_global.h"

void release_pthread_resource(int signo);

extern pthread_mutex_t mutex_client,
                mutex_refresh,
                mutex_transfer,
                mutex_dac;

extern pthread_cond_t cond_client,
                cond_refresh,
                cond_transfer,
                cond_dac;

extern int msgid;
extern int shmid;
extern int semid;

pthread_t id_client, 
        id_refresh,
        id_transfer,
        id_dac;

int main(int argc,const char* argv[])
{
    //初始化锁
    pthread_mutex_init(&mutex_client,NULL);
    pthread_mutex_init(&mutex_refresh,NULL);
    pthread_mutex_init(&mutex_transfer,NULL);
    pthread_mutex_init(&mutex_dac,NULL);
    //等待接受信号，信号处理函数
	signal (SIGINT, release_pthread_resource);
    //初始化各种条件变量
    pthread_cond_init(&cond_client,NULL);
    pthread_cond_init(&cond_refresh,NULL);
    pthread_cond_init(&cond_transfer,NULL);
    pthread_cond_init(&cond_dac,NULL);
    //生成线程
    pthread_create(&id_client,		NULL,pthread_client,NULL);
    pthread_create(&id_refresh,		NULL,pthread_refresh,NULL);
    pthread_create(&id_transfer,	NULL,pthread_transfer,NULL);
    pthread_create(&id_dac,	        NULL,pthread_dac,NULL);
    //等待线程退出
    pthread_join(id_client,NULL);          	    printf ("pthread_client\n");
    pthread_join(id_refresh,NULL);          	printf ("pthread_refresh\n");
    pthread_join(id_transfer,NULL);          	printf ("pthread_transfer\n");
    pthread_join(id_dac,NULL);          	    printf ("pthread_dac\n");

    return 0;
}

void release_pthread_resource(int signo)
{
    //释放锁和条件变量
    pthread_mutex_destroy (&mutex_client);
    pthread_mutex_destroy (&mutex_refresh);
    pthread_mutex_destroy (&mutex_transfer);
    pthread_mutex_destroy (&mutex_dac);

    pthread_cond_destroy (&cond_client);
    pthread_cond_destroy (&cond_refresh);
    pthread_cond_destroy (&cond_transfer);
    pthread_cond_destroy (&cond_dac);

    //分离线程的资源
    pthread_detach(id_client);
    pthread_detach(id_refresh);
    pthread_detach(id_transfer);
    pthread_detach(id_dac);

    printf("all pthread is detached\n");

    msgctl (msgid, IPC_RMID, NULL);
	shmctl (shmid, IPC_RMID, NULL);
	semctl (semid, 1, IPC_RMID, NULL);

	 exit(0);
}