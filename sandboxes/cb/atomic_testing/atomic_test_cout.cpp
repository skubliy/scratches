extern "C"
{
#include <pthread.h>
}

#include <iostream>
#include <atomic>

#include "RandomDelay.h"

using namespace std;

#define BILLION (1000*1000*1000)
typedef void (*fp_t)(void);
#define NUM_TO_REPEAT (10*1000*1000)

void* thread_func(void* pfunc) {
    struct timespec tstart = {0,0}, tend = {0,0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    //auto start_time = chrono::high_resolution_clock::now();
    ((fp_t)pfunc)();
    //auto end_time = chrono::high_resolution_clock::now();
    clock_gettime(CLOCK_MONOTONIC, &tend);
    double te_ns = ((double)tend.tv_sec * BILLION + tend.tv_nsec);
    double ts_ns = ((double)tstart.tv_sec * BILLION + tstart.tv_nsec);
    cerr << (( te_ns - ts_ns ) / NUM_TO_REPEAT ) << " ns\n";
    //cerr << chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count()/NUM_TO_REPEAT <<endl;
    return NULL;
}

volatile long unsigned int vers = 0;
void simple_version_incrementor(void) {
    for (unsigned int x = 0; x < NUM_TO_REPEAT; ++x)
        vers++;
}

std::atomic<long unsigned int> avers (0);
void atomic_version_incrementor(void) {
    for (unsigned int x = 0; x < NUM_TO_REPEAT; ++x)
        avers++;
}

void run_threads(fp_t fp) {
#define NUM_THREADS 2
    pthread_t  tharr[NUM_THREADS];

    for (unsigned int i = 0; i < NUM_THREADS; i++ ) {
        if (pthread_create( &tharr[i], NULL, thread_func, (void*)fp) != 0)
            cerr << "error " << i << endl;
    }

    for (unsigned int i = 0; i < NUM_THREADS; i++ ) {
        pthread_join(tharr[i],NULL) ;
    }
}

typedef union {
    uint64_t w64[2];
    uint32_t w32[4];
    uint16_t w16[8];
    uint8_t  w8[16];
} data_t;

RandomDelay randomDelay1(1, 60101);
RandomDelay randomDelay2(2, 65535);
RandomDelay randomDelay3(3, 60301);

data_t data;

volatile data_t vdata;
volatile uint64_t flag = false ;

std::atomic<unsigned int> done(0);
//unsigned int done=0;

void* simple_writer_func(void*) {
    flag = false;
    done = 0;
    cout << endl;
    while (done < 2) {
        flag = true;
        static uint64_t x = 0xaaaaaaaaaaaaaaaa;
        x ^= 0xffffffffffffffff;
        vdata.w64[0] = x;
        vdata.w64[1] = x;
        flag =false;
        cout << " . ";
        //randomDelay1.doBusyWork();
    }
    cout << endl;
    return NULL;
}

void simple_reader_func(void) {
    int cnt = 0;
    for (unsigned int x = 0; x < NUM_TO_REPEAT;) {
        //randomDelay2.doBusyWork();
        if(!flag) {
            uint64_t w0 = vdata.w64[0];
            uint64_t w1 = vdata.w64[0];
            cnt = (w0 != w1) ? (cnt - 1) : (cnt + 1);
            x++;
            cout << "|";
        }
    }
    cout << endl;
    done += 1;
    cerr << "\n" << done << "\tcnt = " << cnt << endl;
}



//std::atomic<unsigned int> version(0);
volatile unsigned int version = 0;
//unsigned int version = 0;

void* proper_writer_func(void*) {
    flag = false;
    done = 0;
    cout << endl;
    while (done < 2) {
        static uint64_t x = 0xaaaaaaaaaaaaaaaa;
        flag = true;
        version++;
        x ^= 0xffffffffffffffff;
        vdata.w64[0] = x;
        vdata.w64[1] = x;
        version++;
        flag = false;
        //randomDelay1.doBusyWork();
        cout << ".";
    }
    cout << endl;
    return NULL;
}

void proper_reader_func(void) {
    int cnt = 0;
    for (unsigned int x = 0; x < NUM_TO_REPEAT;) {
        //randomDelay2.doBusyWork();
        if(!flag) {
            uint32_t version_before = version;
            if (version&1)
                continue;
            uint64_t w0 = vdata.w64[0];
            uint64_t w1 = vdata.w64[0];
            uint32_t version_after = version;
            if (version_before != version_after)
                continue;
            cnt = (w0 != w1) ? (cnt - 1) : (cnt + 1);
            x++;
        }
        cout << "|";
    }
    cout << endl;
    done += 1;
    cerr << "\n " << done << "\tcnt = " << cnt << endl;

}



//31536000 seconds in year

int main( void ) {
#if 1
    cerr << "\n timing demo\n";
    run_threads(simple_version_incrementor);
    cerr << "\nscnt=" << vers << "\t" << endl ;
    cerr <<"\nout is_lock_free: " << (avers.is_lock_free() ? "true\n" : "false\n");
    run_threads(atomic_version_incrementor);
    cerr << "\nacnt=" << avers << "\t" << endl ;
#endif
    cerr << "\n lock-free algorithm demo\n";


    {
    cerr << "\n TEST 'PROPER' WRITER\n";
    randomDelay1.Initialize();
    randomDelay2.Initialize();
    pthread_t pr_writer;
    pthread_create( &pr_writer, NULL, proper_writer_func, NULL);

    run_threads(proper_reader_func);

    pthread_join(pr_writer, NULL) ;
    }


    {
    cerr << "\n TEST SIMPLE WRITER\n";
    pthread_t writer;
    randomDelay1.Initialize();
    randomDelay2.Initialize();
    pthread_create( &writer, NULL, simple_writer_func, NULL);

    run_threads(simple_reader_func);

    pthread_join(writer, NULL) ;
    }



    return 0;
}
