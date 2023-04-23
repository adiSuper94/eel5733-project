#include <linux/ioctl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/a5"

#define CDRV_IOC_MAGIC 'Z'
#define E2_IOCMODE1 _IO(CDRV_IOC_MAGIC, 1)
#define E2_IOCMODE2 _IO(CDRV_IOC_MAGIC, 2)


int main(int argc, char *argv[]) {
	printf("opening device forst time\n");
	int fd1 = open(DEVICE,O_RDWR);
	printf("opening device second time\n");
	int fd2 = open(DEVICE, O_RDWR);
	printf("closing fd1\n");
	close(fd1);
	printf("closing fd2\n");
	close(fd2);
}
