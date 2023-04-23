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

void *openDev(){
	sleep(3);
	int fd = open(DEVICE, O_RDWR); // this thread should be waiting at this line.
	
	if(fd < 0){
		printf("Error while opening file.");
		exit(-1);
	}

	close(fd);	
	printf("Device opened and closed\n ");
	pthread_exit(NULL);
}

void *switchMode2(){
	int fd = open(DEVICE, O_RDWR);
	if(fd < 0){
		printf("Error while opening file.");
		exit(-1);
	}

	sleep(2);
	printf("Switching to MODE2\n");
	ioctl(fd,E2_IOCMODE2,0); // This thread should be waiting at this line
	close(fd);	
	printf("Thread2 finished\n");
	pthread_exit(NULL);
	
}

int  main(){
	pthread_t t1, t2;
	pthread_create(&t1, NULL, switchMode2, NULL);
	pthread_create(&t2, NULL, openDev, NULL);
	pthread_exit(NULL);

}
