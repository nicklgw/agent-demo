
#ifndef __AGENT_H__
#define __AGENT_H__

#include <poll.h>

#define MQTT_BROKER_IP		"localhost"
#define MQTT_BROKER_PORT	1883
#define MQTT_KEEPALIVE		60

//#define POLL_TIMEOUT		100	/* 100ms */
#define POLL_TIMEOUT		1000	/* 100ms */
#define MAX_CLIENT_NUM		20
#define MAX_POLL_FDS		(MAX_CLIENT_NUM + 3)
#define TIMER_INTERVAL		1000	/* 1s */

struct agent_info
{
	struct pollfd pollfds[MAX_POLL_FDS];
	int count_pollfds;
	
	class mosquittopp_cloud *mosq;
	
	int mosq_sock;
	int timer_fd;
};

struct agent_info* get_app(void);

int delete_fd_from_agent(int sockfd);

#endif // __AGENT_H__

