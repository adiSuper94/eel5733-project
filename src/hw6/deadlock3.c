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

void *runner(){
	int fd = open(DEVICE, O_RDWR);
	sleep(1);
	printf("Switching to MODE2\n");
	ioctl(fd,E2_IOCMODE1,0); // This thread should be waiting at this line
	close(fd);	
	printf("Thread finished\n");
	pthread_exit(NULL);
	
}

int  main(){
	pthread_t t1, t2;
	int fd = open(DEVICE, O_RDWR);
	printf("Main: witching to MODE2\n");
	ioctl(fd,E2_IOCMODE2,0);
	close(fd);
	pthread_create(&t1, NULL, runner, NULL);
	pthread_create(&t2, NULL, runner, NULL);
	pthread_exit(NULL);

}
