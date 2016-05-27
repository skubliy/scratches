#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <atomic>

#define NUM_REPEAT (1000*1000)
template <typename TV>
void check_speed(unsigned long nrep = NUM_REPEAT) {
    static const unsigned long long nano = 1000000000;
    unsigned long long t1, t2;
    double t0;

    struct timespec tm;
    clock_gettime( CLOCK_REALTIME, &tm );
    t1 = tm.tv_nsec + tm.tv_sec * nano;
    static TV x(0);
    for (int i=0; i < nrep; i++) {
        x++;
    }

    clock_gettime( CLOCK_REALTIME, &tm );
    t2 = tm.tv_nsec + tm.tv_sec * nano;
    t0= (t2 -t1);
    fprintf( stdout,"\n%lld - %lld = %f ns\n%f ns\n", t2, t1, t0, t0/nrep );
}

int main( int argc, char ** argv ) {

    check_speed<unsigned long long>();
    check_speed<volatile unsigned long long>();

    typedef std::atomic<unsigned long long> atomic_t;

    check_speed< std::atomic<unsigned long long> >();

    return 0;

}
