/*
 * profile.h
 *
 *  Created on: 22 sty 2016
 *      Author: m-swacha
 */

#ifndef PROFILE_H_
#define PROFILE_H_

#include <unistd.h>
#include <sys/time.h>

#define START struct timeval t1, t2; \
(void)gettimeofday(&t1, 0);

#define STOP(x) (void)gettimeofday(&t2, 0); \
    x += time_diff(&t2, &t1);

#endif /* PROFILE_H_ */

int time_diff(struct timeval *t2, struct timeval *t1);
