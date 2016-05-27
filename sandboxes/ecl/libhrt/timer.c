/*
 *  timer.c
 *  libhrt
 *
 *  Created by Greg Hookey on 20/04/06.
 *  Copyright 2006 DeadlySoft. All rights reserved.
 *
 */

#include "timer.h"


struct hrt_timer_info {
	/*  conversion factor constants */
	double	raw_to_nano_secs_factor;
	double	nano_secs_to_micro_secs_factor;
	double	micro_secs_to_milli_secs_factor;
	double	milli_secs_to_seconds_factor;
	double	raw_to_micro_secs_factor;
	double	raw_to_milli_secs_factor;
    double  raw_to_seconds_factor;
    int64_t raw_system_start_ticks;
};


/* global variables */

static struct hrt_timer_info g_timer_info;


/* static method prototypes */

static int64_t determine_tsc_frequency ( void );
static int64_t determine_system_start_epoch_ticks ( void );

#if defined(linux) || defined(__linux__)
	#if defined(__x86_64__) || defined(__i386__)
		static int64_t read_tsc ( void );
	#else
		#error "Platform Not Supported"
	#endif
#else
	#error "Platform Not Supported"
#endif


/* inline and other functions */

inline
double hrt_to_nano_seconds ( int64_t raw_value )
{
	return( (double)raw_value * g_timer_info.raw_to_nano_secs_factor );
}

inline
double hrt_to_micro_seconds ( int64_t raw_value )
{
	return( (double)raw_value * g_timer_info.raw_to_micro_secs_factor );
}

inline
double hrt_to_milli_seconds ( int64_t raw_value )
{
	return( (double)raw_value * g_timer_info.raw_to_milli_secs_factor );
}

inline
double hrt_to_seconds ( int64_t raw_value )
{
	return( (double)raw_value * g_timer_info.raw_to_seconds_factor );
}

inline
int64_t hrt_from_nano_seconds ( double nano_secs )
{
	return( (int64_t)( nano_secs / g_timer_info.raw_to_nano_secs_factor ) );
}

inline
int64_t hrt_from_micro_seconds ( double micro_secs )
{
	return( (int64_t)( micro_secs / g_timer_info.raw_to_micro_secs_factor ) );
}

inline
int64_t hrt_from_milli_seconds ( double milli_secs )
{
	return( (int64_t)( milli_secs / g_timer_info.raw_to_milli_secs_factor ) );
}

inline
int64_t hrt_from_seconds ( double secs )
{
	return( (int64_t)( secs / g_timer_info.raw_to_seconds_factor ) );
}

void hrt_timer_reset ( hrt_timer * timer )
{
	if ( !timer ) {
		fprintf( stderr, "attempted to reset an invalid timer." );
		return;
	}

	*timer = read_tsc();
}

int64_t hrt_timer_get_elapsed ( const hrt_timer * timer )
{
	int64_t now = 0;

#if defined(linux) || defined(__linux__)
	#if defined(__x86_64__) || defined(__i386__)
		now = read_tsc();
	#else
		#error "Platform Not Supported"
	#endif
#else
	#error "Platform Not Supported"
#endif

	return( now - *timer );
}

int64_t hrt_timer_now ( void )
{
	int64_t now = 0;

#if defined(linux) || defined(__linux__)
	#if defined(__x86_64__) || defined(__i386__)
		now = read_tsc();
	#else
		#error "Platform Not Supported"
	#endif
#else
	#error "Platform Not Supported"
#endif

	return( now );
}

bool hrt_timer_check ( const hrt_timer * timer, int64_t raw_value )
{
	if ( hrt_timer_get_elapsed( timer ) >= raw_value ) return ( true );

	return( false );
}

void hrt_sync_clock( void )
{
    g_timer_info.raw_system_start_ticks += hrt_from_seconds( time( NULL ) - hrt_to_seconds( hrt_timer_now() ) );
}

//
// static methods
//

#if (defined(linux) || defined(__linux__)) && defined(__x86_64__)
__inline__ static uint64_t rdtsc(void)
{
    uint32_t lo, hi;
    __asm__ __volatile__ (      // serialize
    "xorl %%eax,%%eax \n        cpuid"
    ::: "%rax", "%rbx", "%rcx", "%rdx");
    /* We cannot use "=A", since this would use %rax on x86_64 */
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return (uint64_t)hi << 32 | lo;
}
#else
    #error "Platform Not Supported"
#endif


static int64_t read_tsc ( void )
{
    return rdtsc() + g_timer_info.raw_system_start_ticks;
}


static const char * sCpuInfoFilename = "/proc/cpuinfo";

static int64_t determine_tsc_frequency ( void )
{
	int64_t tsc_frequency = 0;

	#define LINE_BUF_SIZE (1024)
	char buf[ LINE_BUF_SIZE	] = { 0 };
	char * ptr = 0;
	double cpu_mhz = 0.0f;

	FILE *fh = fopen( sCpuInfoFilename, "r" );
	if ( fh == NULL ) {
		fprintf( stderr, "Unable to open \"%s\" for reading.\n", sCpuInfoFilename );
	} else {
	    while ( fgets( buf, sizeof( buf ), fh ) != NULL ) {
            ptr = strstr( buf, "cpu MHz" );
            if ( !ptr ) continue;
            sscanf( ptr, "cpu MHz : %lf", &cpu_mhz );
            //fprintf( stdout, "CPU Mhz: %lf\n", cpu_mhz );
            break;
	    }
	    fclose( fh );
	}

	if ( cpu_mhz < 200.0 || cpu_mhz > 30000.0 ) {
	    // bug #5554: don't return a value which is so far out of bounds
	    // that it causes the rest of the system to misbehave
	    fprintf( stderr, "Invalid cpu frequency (%f).  Defaulting to 200MHz clock speed.\n", cpu_mhz );
	    cpu_mhz = 200.0;
	}

    tsc_frequency = (int64_t)( cpu_mhz * 1000000.0f );

	return tsc_frequency;
}

static const char * sUptimeFilename = "/proc/uptime";

static int64_t determine_system_start_epoch_ticks ( void )
{
    #define LINE_BUF_SIZE (1024)
    char buf[ LINE_BUF_SIZE ] = { 0 };
    double systemSecs = 0.0f;
    double idleSecs = 0.0f;

    FILE * fh = 0;

    fh = fopen( sUptimeFilename, "r" );
    if ( !fh ) {
        fprintf( stderr, "Unable to open %s for reading. Defaulting to now.\n", sCpuInfoFilename );
        return( hrt_from_seconds( time( NULL ) ) );
    }

    if ( fgets( buf, sizeof( buf ), fh ) != NULL ) {
        sscanf( buf, "%lf %lf", &systemSecs, &idleSecs );
    }

    fclose( fh );

    return( hrt_from_seconds( time( NULL ) - systemSecs ) );
}

void hrt_to_timeval ( struct timeval *tv, int64_t raw_value )
{
    // We choose to have all of our times be slightly later rather than
    // earlier by reading the tsc before getting the time of day.
    int64_t now = hrt_timer_now();
    gettimeofday(tv,NULL);
    double adjustment = hrt_to_micro_seconds( now );
    tv->tv_usec -= adjustment;
    tv->tv_sec += tv->tv_usec / 1000000;
    tv->tv_usec %= 1000000;
    if (tv->tv_usec < 0) {
        tv->tv_sec--;
        tv->tv_usec += 1000000;
    }

    adjustment = hrt_to_micro_seconds( raw_value );

    tv->tv_usec += adjustment;
    tv->tv_sec  += (tv->tv_usec / 1000000);
    tv->tv_usec %= 1000000;
}

void hrt_init ( void )
{
    // run 3 passes of rdtsc to warm-up
    rdtsc();
    rdtsc();
    rdtsc();

    g_timer_info.raw_to_nano_secs_factor        = 0.0f;
    g_timer_info.nano_secs_to_micro_secs_factor = 1000.0f;
    g_timer_info.micro_secs_to_milli_secs_factor    = 1000.0f;
    g_timer_info.milli_secs_to_seconds_factor   = 1000.0f;
    g_timer_info.raw_to_micro_secs_factor       = 0.0f;
    g_timer_info.raw_to_milli_secs_factor       = 0.0f;
    g_timer_info.raw_to_seconds_factor      = 0.0f;

#if defined(linux) || defined(__linux__)
    #if defined(__x86_64__) || defined(__i386__)
    // conversion from tsc -> nanoseconds
    // ns = cycles / ( freq / ns_per_sec )
    // ns = cycles * ( ns_per_sec / freq )
    // ns = cycles * ( 10^9 / ( cpu_mhz * 10^6 ) )
    //
    // could be further reduced, but these are the
    // terms in which we are dealing

        double tscFreq = determine_tsc_frequency();
    g_timer_info.raw_to_nano_secs_factor =
                   ( g_timer_info.nano_secs_to_micro_secs_factor *
                 g_timer_info.micro_secs_to_milli_secs_factor *
                 g_timer_info.milli_secs_to_seconds_factor ) /
                 tscFreq;
    #else
        #error "Platform Not Supported"
    #endif
#else
    #error "Platform Not Supported"
#endif
    g_timer_info.raw_to_micro_secs_factor =
        g_timer_info.raw_to_nano_secs_factor / g_timer_info.nano_secs_to_micro_secs_factor;
    g_timer_info.raw_to_milli_secs_factor =
        g_timer_info.raw_to_micro_secs_factor / g_timer_info.micro_secs_to_milli_secs_factor;
    g_timer_info.raw_to_seconds_factor =
        g_timer_info.raw_to_milli_secs_factor / g_timer_info.milli_secs_to_seconds_factor;

    g_timer_info.raw_system_start_ticks = determine_system_start_epoch_ticks();

    hrt_sync_clock( );
}

