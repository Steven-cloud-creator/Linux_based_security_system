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
#include "cgic.h"
#include "data_global.h"

#define N 32

#define MONITOR_NUM 1

struct shm_addr
{
	char shm_status;   //shm_status可以等于home_id，用来区分共享内存数据
	struct env_info  security_env_info;
};

//int main()
int cgiMain()
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


	// printf("%.2f",shm_buf->security_env_info.monitor_no[0].dht11_info.temperature);
	// printf("%.2f",shm_buf->security_env_info.monitor_no[0].dht11_info.humidity);
	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<head><meta http-equiv=\"refresh\" content=\"1\"><style><!--body{line-height:50%}--></style> </head>");
	fprintf(cgiOut, "<HTML>\n");
	fprintf(cgiOut, "<BODY bgcolor=\"#666666\">\n");
	//fprintf(cgiOut, "<h1><font color=\"#FF0000\">HOME_ID #%d:</font></H2>\n ", shm_buf->shm_status);
	if (shm_buf->shm_status == 0)
	{
		fprintf(cgiOut, "<script>function show(){var date =new Date(); var now = \"\"; now = date.getFullYear()+\"年\"; now = now + (date.getMonth()+1)+\"月\"; \ now = now + date.getDate()+\"日\"; now = now + date.getHours()+\"时\"; now = now + date.getMinutes()+\"分\";now = now + date.getSeconds()+\"秒\"; document.getElementById(\"nowDiv\").innerHTML = now; setTimeout(\"show()\",1000);} </script> \n ");	
		fprintf(cgiOut, "<h2><font face=\"Broadway\"><font color=\"#00FAF0\">Home1 Real-time Environment Info:</font></font></H2>\n ");
		fprintf(cgiOut, "<h2 align=center><font color=\"#cc0033\"><body onload=\"show()\"> <div id=\"nowDiv\"></div></font></h2> \n "); 	
		fprintf(cgiOut, "<h4>DHT11数据显示部分</H4>\n ");
		fprintf(cgiOut, "<h4>Temperature:\t%.2f</H4>\n ", shm_buf->security_env_info.monitor_no[0].dht11_info.temperature );
		fprintf(cgiOut, "<h4>Humidity:\t%.2f</H4>\n ", shm_buf->security_env_info.monitor_no[0].dht11_info.humidity);

		fprintf(cgiOut, "<h4>......</H4>\n ");
	}
	else
	{
		fprintf(cgiOut, "<h2><font face=\"Broadway\"><font color=\"#FFFAF0\">Close!</font></font></H2>\n ");	
	}
	//	fprintf(cgiOut, "<h3>:</H3>\n ");	
	fprintf(cgiOut, "</BODY></HTML>\n");	
    sem_v (semid, 0);
	return 0;
}