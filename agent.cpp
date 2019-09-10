
#include <stdio.h>
#include <unistd.h>

#include "agent.h"

static struct agent_info the_app = { 
	
	
	
};

struct agent_info* get_app(void)
{
	return &the_app;
}

int delete_fd_from_agent(int sockfd)
{
	int i;
	
	close(sockfd);
	
	for (i = 0; i < get_app()->count_pollfds; i++) 
	{
		if (get_app()->pollfds[i].fd == sockfd)
			break;
	}
	
	printf("%s(), sockfd: %d, i: %d\n", __func__, sockfd, i);
	
	if (i == get_app()->count_pollfds) return -1;
	
	while (i < get_app()->count_pollfds - 1 && get_app()->pollfds[i].fd) 
	{
		get_app()->pollfds[i] = get_app()->pollfds[i + 1];
		i++;
	}
	
	get_app()->pollfds[i].fd = -1;
	get_app()->count_pollfds--;
	
	return 0;
}

