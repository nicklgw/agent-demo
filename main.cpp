
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "agent.h"
#include "cloud.h"
#include "timer.h"

int main(int argc, char *argv[])
{
	mosqpp::lib_init();
	
	int run = -1;
	struct mosquittopp_cloud *mosq;
	
	mosq = new mosquittopp_cloud();
	
	int ret = mosq->connect(MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_KEEPALIVE);
	if(MOSQ_ERR_SUCCESS != ret)
	{
		printf("Failed to connect MQTT Broker. \n");
		return -1;
	}
	
	get_app()->mosq = mosq;
	get_app()->mosq_sock = get_app()->mosq->socket();
	printf(" socket: %d \n", get_app()->mosq->socket());
	
	get_app()->pollfds[get_app()->count_pollfds].fd = get_app()->mosq_sock;
	get_app()->pollfds[get_app()->count_pollfds].events = POLLIN | POLLOUT;
	get_app()->count_pollfds++;
	
	int timer_fd = timer_setup();
	get_app()->timer_fd = timer_fd;
	assert(timer_fd > 0);
	printf(" timer_fd: %d \n", timer_fd);
	timer_start(timer_fd, TIMER_INTERVAL, TIMER_INTERVAL);
	get_app()->pollfds[get_app()->count_pollfds].fd = timer_fd;
	get_app()->pollfds[get_app()->count_pollfds].events = POLLIN;
	get_app()->count_pollfds++;
	
	int n = 0;
	while (n >= 0) 
	{
		if(get_app()->mosq->want_write())
			get_app()->pollfds[0].events = POLLIN | POLLOUT;
		else
			get_app()->pollfds[0].events = POLLIN;
		
		int i;
		
		n = poll(get_app()->pollfds, get_app()->count_pollfds, POLL_TIMEOUT);
		if (n < 0) 
		{
			perror("poll \n");
			continue;
		}
		
		if (n == 0) 
		{
			printf("poll timeout \n");
			continue;
		}
		
		for (i = 0; i < get_app()->count_pollfds && n > 0; i++) 
		{
			if (get_app()->pollfds[i].revents & (POLLNVAL | POLLHUP | POLLERR)) 
			{
				int j = i;
				
				printf("(POLLNVAL | POLLHUP | POLLERR) \n");
				printf("pollfds[%d].revents: %08x, %d\n",
					i, get_app()->pollfds[i].revents, get_app()->pollfds[i].fd);
				
				delete_fd_from_agent(get_app()->pollfds[i].fd);
				n--;
			}
			else if (get_app()->pollfds[i].revents & POLLIN) 
			{
				printf("POLLIN \n");
				printf("pollfds[%d].revents: %08x, %d\n",
					i, get_app()->pollfds[i].revents, get_app()->pollfds[i].fd);
				
				if (get_app()->pollfds[i].fd == get_app()->timer_fd)
				{
					timer_handler(timer_fd);
				}
				else if (get_app()->pollfds[i].fd == get_app()->mosq_sock)
				{
					get_app()->mosq->loop_read(1);
				}
				else
				{
					
				}
				
				n--;
			}
			else if (get_app()->pollfds[i].revents & POLLOUT) 
			{
				printf("POLLOUT \n");
				printf("pollfds[%d].revents: %08x, %d\n",
					i, get_app()->pollfds[i].revents, get_app()->pollfds[i].fd);
				
				get_app()->mosq->loop_write(1);
				
				n--;
			}
			
		}
		
		get_app()->mosq->loop_misc();
	}
	
	mosqpp::lib_cleanup();
	
	return 0;
}

