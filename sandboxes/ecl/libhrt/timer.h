/*
 *  timer.h
 *  libhrt
 *
 *  Created by Greg Hookey on 20/04/06.
 *  Copyright 2006 DeadlySoft. All rights reserved.
 *
 */

#ifndef __LIBRT_TIMER_H__
#define __LIBRT_TIMER_H__

#include "common.h"

typedef int64_t hrt_timer;

#    ifdef __cplusplus
extern "C" {
#    endif


/* timer library initialization method */
void    hrt_init( void );

// Adjust the number of ticks since epoch. Allows for the condition
//     hrt_to_seconds( hrt_timer_now() ) == time( NULL )
//     with in 0.001 seconds
void hrt_sync_clock( void );

/* timer control functions */
void 	hrt_timer_reset ( hrt_timer * timer );
bool 	hrt_timer_check ( const hrt_timer * timer, int64_t raw_value );
int64_t hrt_timer_get_elapsed ( const hrt_timer * timer );
int64_t hrt_timer_now ( void );

/* conversion convenience methods */

double  hrt_to_nano_seconds ( int64_t raw_value );
double  hrt_to_micro_seconds ( int64_t raw_value );
double  hrt_to_milli_seconds ( int64_t raw_value );
double  hrt_to_seconds ( int64_t raw_value );
void    hrt_to_timeval ( struct timeval *tv, int64_t raw_value );

int64_t  hrt_from_nano_seconds ( double nano_secs );
int64_t  hrt_from_micro_seconds ( double micro_secs );
int64_t  hrt_from_milli_seconds ( double milli_secs );
int64_t  hrt_from_seconds ( double secs );

#    ifdef __cplusplus
}
#    endif

#endif
