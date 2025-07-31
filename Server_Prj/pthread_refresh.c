#include "data_global.h"
#include "sem.h"

#define N 1024  //for share memory

extern int semid;       //信号量集标识符
extern int shmid;       //共享内存标识符

extern key_t sem_key;   //共享信号量键值
extern key_t shm_key;   //共享内存键值

extern pthread_mutex_t mutex_refresh;

extern pthread_cond_t cond_refresh;

extern struct env_info security_env_info;

struct shm_addr
{
	char shm_status;   //shm_status可以等于home_id，用来区分共享内存数据
	struct env_info  security_env_info;
};
struct shm_addr *shm_buf;

int file_env_info_struct(struct env_info  *rt_status,int home_id);//模拟数据刷新的函数

void *pthread_refresh(void *arg)
{
    //生成信号量键值
    if((sem_key = ftok("/tmp",'i')) < 0){
		perror("ftok failed .\n");
		exit(-1);
	}
	printf("key = %x\n",sem_key);
    //创建信号量集
    semid = semget(sem_key,1,IPC_CREAT|IPC_EXCL|0666);
	if(semid == -1)	{
		if(errno == EEXIST){
			semid = semget(sem_key,1,0777);
		}else{
			perror("fail to semget");
			exit(1);
		}
	}else{
		init_sem(semid, 0, 1);
	}
    //生成共享内存键值
    if((shm_key = ftok("/tmp",'i')) < 0){
		perror("ftok failed .\n");
		exit(-1);
	}
    //创建共享内存段
    shmid = shmget(shm_key,N,IPC_CREAT|IPC_EXCL|0666);
	if(shmid == -1)	{
		if(errno == EEXIST){
			shmid = shmget(shm_key,N,0777);
		}else{
			perror("fail to shmget");
			exit(1);
		}
	}
    //共享内存
    if((shm_buf = (struct shm_addr *)shmat(shmid,NULL,0)) == (void *)-1)
	{
		perror("fail to shmat");
		exit(1);
	}
	printf("pthread_refresh ......>>>>>>>\n");
	bzero (shm_buf, sizeof (struct shm_addr));
		while(1){
		sem_p(semid,0); //P操作
		shm_buf->shm_status = 0;
		int home_id = 0;
#if 1
		shm_buf->security_env_info.monitor_no[home_id] = security_env_info.monitor_no[home_id];  //真实数据上传
#else
		file_env_info_struct(&shm_buf->security_env_info,shm_buf->shm_status); //模拟数据上传
#endif 
		sleep(1);
		sem_v(semid,0); //v操作
		pthread_cond_signal(&cond_transfer);
	}
}

int file_env_info_struct(struct env_info *rt_status,int home_id)
{
	int env_info_size = sizeof(struct env_info);
	printf("env_info_size = %d.\n",env_info_size);
	rt_status->monitor_no[home_id].dht11_info.humidity = 10;
	rt_status->monitor_no[home_id].dht11_info.temperature = 10;
	//添加stm32部分的数据、arduino数据，
	return 0;
}


