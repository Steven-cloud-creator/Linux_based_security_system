#include "data_global.h"


extern pthread_mutex_t  mutex_dac;
extern pthread_cond_t   cond_dac;

extern unsigned int cmd_dac;

int dac_fd;
unsigned int val;
int status;
#define SPI_IOC_WR 123

void *pthread_dac(void *arg)
{
    printf("pthread_dac\n");

    dac_fd = open(DAC_DEV, O_RDWR);
    if (dac_fd < 0) {
		printf("can not open %s\n", DAC_DEV);
		return;
	}
    printf("dac_fd : %d.\n",dac_fd);

    while(1)
    {
        pthread_mutex_lock(&mutex_dac);
        pthread_cond_wait(&cond_dac,&mutex_dac);
        val = cmd_dac;
	    status = ioctl(dac_fd, SPI_IOC_WR, &val);
	    if (status < 0) {
		    printf("SPI_IOC_WR\n");
        }
        pthread_mutex_unlock(&mutex_dac);
        /* 打印 */
	    printf("Pre val = %d\n", val);
	}
	
    close(dac_fd);
}