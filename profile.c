/*
 * profile.c
 *
 *  Created on: 22 sty 2016
 *      Author: m-swacha
 */

#include "profile.h"

int time_diff(struct timeval *t2, struct timeval *t1)
{
    int r;

    r = (t2->tv_sec - t1->tv_sec)*1000000 + t2->tv_usec- t1->tv_usec;

    return(r);
}
