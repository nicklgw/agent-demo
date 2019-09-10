
#ifndef __TIMER_H__
#define __TIMER_H__

#include <sys/timerfd.h>
#include <stdint.h>

int timer_setup(void);

int timer_start(int fd, int first_expire, int interval);

void timer_handler(int fd);

#endif // __TIMER_H__

