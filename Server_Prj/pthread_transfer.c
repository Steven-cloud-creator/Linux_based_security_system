#include "data_global.h"

int dht11_fd;

extern pthread_mutex_t mutex_transfer;
extern pthread_cond_t cond_transfer;
extern struct env_info security_env_info;

int env_info_get(struct env_info* info,int home_id);

void *pthread_transfer(void* arg)
{
    int home_id = 0;
    dht11_fd = open(DHT11_DEV,O_RDWR);
    
    if(dht11_fd == -1)
    {
        printf("can not open file %s\n",DHT11_DEV);
        return;
    }
    while(1)
    {
        printf("transfer");
        pthread_mutex_lock(&mutex_transfer);
        pthread_cond_wait(&cond_transfer,&mutex_transfer);
        printf("pthread_analysis and tranfer.\n");
        env_info_get(&security_env_info,home_id);
        pthread_mutex_unlock(&mutex_transfer);
        sleep(1);
    }
    close(dht11_fd);
}

int env_info_get(struct env_info* info,int home_id)
{
    unsigned char data[4];
    if (read(dht11_fd, data, 4) == 4)
	{
		printf("get humidity  : %.2f\n", (float)data[0]+(float)(data[1]*0.01));
		printf("get temprature: %.2f\n", (float)data[2]+(float)(data[3]*0.01));
        info->monitor_no[home_id].dht11_info.humidity = (float)data[0]+(float)(data[1]*0.01);
        info->monitor_no[home_id].dht11_info.temperature = (float)data[2]+(float)(data[3]*0.01);
	}
	else 
	{
		printf("get humidity/temprature: -1\n");
	}
    return 0;
}