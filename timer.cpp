
#include <sys/timerfd.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "timer.h"

int timer_setup(void)
{
	int fd = -1;
	
	fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (fd < 0) 
	{
		perror("timerfd_create");
		return fd;
	}
	
	return fd;
}

int timer_start(int fd, int first_expire, int interval)
{
	struct itimerspec new_value;
	
	new_value.it_value.tv_sec = first_expire / 1000;
	new_value.it_value.tv_nsec = first_expire % 1000 * 1000000;
	
	new_value.it_interval.tv_sec = interval / 1000;
	new_value.it_interval.tv_nsec = interval % 1000 * 1000000;
	
	if (timerfd_settime(fd, 0, &new_value, NULL) == -1) 
	{
		perror("timerfd_settime");
		return -1;
	}
	
	return 0;
}

void timer_handler(int fd)
{
	printf("timer_handler time: %ld \n", time(NULL));
	
	uint64_t exp = 0;
	static uint32_t i = 0;
	/* just read out the "events" in fd, otherwise poll will keep
	 * reporting POLLIN */
	read(fd, &exp, sizeof(uint64_t));

//	if (miot_fd <= 0) 
//	{
//		miot_fd = miot_connect_init();
//		agent.pollfds[0].fd = miot_fd;
//	}
	
	i++;
	if (i % 100 == 0) 
	{
		printf("timer_handler: update_id_tree()");
		i = 0;
	}
}





