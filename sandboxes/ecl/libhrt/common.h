/*
 *  common.h
 *  libhrt
 *
 *  Created by Greg Hookey on 20/04/06.
 *  Copyright 2006 DeadlySoft. All rights reserved.
 *
 */

#ifndef __LIBRT_COMMON_H__
#define __LIBRT_COMMON_H__

#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#if defined(linux) || defined(__linux__) 
	#if defined(__x86_64__) || defined(__i386__)
		#include <stdint.h>
		#include <stdio.h>
		#include <string.h>
                #include <stdbool.h>
	#else
		#error "Platform Not Supported"
	#endif
#else
	#error "Platform Not Supported"
#endif

#endif

