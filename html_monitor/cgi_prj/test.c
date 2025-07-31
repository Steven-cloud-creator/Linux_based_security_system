#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>
#include <unistd.h>
#include "sem.h"
#include "data_global.h"

#define N 32

#define MONITOR_NUM 1

struct shm_addr
{
	char shm_status;   //shm_status可以等于home_id，用来区分共享内存数据
	struct env_info  security_env_info;
};

int main()
{
    key_t shm_key;
    key_t sem_key;
	int shmid,semid;
	struct shm_addr *shm_buf;

    if((sem_key = ftok("/tmp",'i')) <0)
	{
		perror("ftok");
		exit(1);
	}
	printf("key = %x\n",sem_key);

	if((semid  = semget(sem_key, 1, 0666)) < 0)
	{
		perror("semget");
		exit(1);
	}

    if((shm_key = ftok("/tmp",'i')) < 0){
		perror("ftok failed .\n");
		exit(-1);
	}

	if((shmid = shmget(shm_key, N, 0666 )) == -1)
	{
		perror("shmget");
		exit(1);
	}

	if((shm_buf = (struct shm_addr*)shmat(shmid, NULL, 0)) == (void*)-1 )
	{
		perror("shmat");
		exit(1);
	}

	sem_p(semid,0);
    
    printf("%d",shm_buf->shm_status);
	if (shm_buf->shm_status == 0)
	{
        printf("Temperature:%d\n",shm_buf->security_env_info.monitor_no[0].dht11_info.temperature);
        printf("Temperature:%d\n",shm_buf->security_env_info.monitor_no[0].dht11_info.humidity);
	}
    sem_v (semid, 0);
	return 0;
}