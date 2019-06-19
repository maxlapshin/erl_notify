#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <poll.h>
#include <pthread.h>



void *poller(void *arg) {
	int trigger_fd;
	struct pollfd notifies[1];
	char value[20];

	trigger_fd = open("/sys/kernel/erl_notify/trigger", O_RDONLY);

	notifies[0].fd = trigger_fd;
	notifies[0].events = POLLPRI|POLLERR;

	while(1) {
		notifies[0].revents = 0;
		poll(notifies, 1, -1);
		lseek(trigger_fd,0,SEEK_SET);
		read(trigger_fd,value,sizeof(value));
		printf("polled %s",value);
	}
}

int main(int argc, char **argv)
{
	pthread_t poll_thr;
	pthread_create(&poll_thr,0,poller,0);

	for(int i = 1; ;i++) {
		char buf[20];
		sprintf(buf,"%d\n",i);
		int fd = open("/sys/kernel/erl_notify/trigger", O_WRONLY);
		write(fd,buf,strlen(buf));
		close(fd);
		sleep(2);
	}

}


