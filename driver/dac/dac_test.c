

/* 参考: tools\spi\spidev_fdx.c */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>

#define SPI_IOC_WR 123

/* dac_test /dev/spidevB.D <val> */

int main(int argc, char **argv)
{
	int fd;
	unsigned int val;
	int status;
	
	if (argc != 3)
	{
		printf("Usage: %s /dev/spidevB.D <val>\n", argv[0]);
		return 0;
	}

	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf("can not open %s\n", argv[1]);
		return 1;
	}

	val = strtoul(argv[2], NULL, 0);

	status = ioctl(fd, SPI_IOC_WR, &val);
	if (status < 0) {
		printf("SPI_IOC_WR\n");
		return -1;
	}

	/* 打印 */
	printf("Pre val = %d\n", val);
	
	
	return 0;
}

