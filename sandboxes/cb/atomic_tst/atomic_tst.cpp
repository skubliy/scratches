extern "C"
{
#include <pthread.h>
}

#include <iostream>
#include <atomic>

using namespace std;

#define NUM_TO_REPEAT (10*1000)

template<typename TA>
class tester {
public:

    typedef void (*func_t)(TA&, uint32_t);
    typedef void* (tester<TA>::* pthf_t)(void*);
    typedef void* (* ppp)(void*);
private:
    static TA var;
    //uint32_t nthrds;



    struct executor {
        func_t pfunc;
        uint64_t nrep;
        uint32_t nthrds;
        executor(func_t pf, uint64_t nr, uint32_t nth = 1)
            : pfunc(pf), nrep(nr), nthrds(nth) {}
    };

    executor exr;
    executor exw;

public:
    //static TA var;
    tester(uint32_t nr, uint32_t nth, func_t fr, func_t fw = NULL)
        : exr(fr, nr, nth), exw(fw, nr) {
        //ppf = &tester<TA>::thread_func;
        tester<TA>::var = 0;
    }

    void run() {
        pthread_t thr_writer = 0;

        if (exw.pfunc) {
            //thf_t pf = (thf_t)(this->thread_func);
            pthread_create( &thr_writer, NULL, (ppp)&tester<TA>::thread_func, (void*)&exw);
        }

        run_readers(exr);

        if (thr_writer) {
            pthread_join(thr_writer, NULL) ;
        }
    }

private:
    static void* thread_func(void* p) {
        if (p == NULL )
            return NULL;
        executor& ex = *(executor*)p;
        fprintf(stdout, "%p p=%p \tex=%p \n", &tester<TA>::thread_func, p, &ex);
        if (ex.pfunc == NULL)
            return NULL;

        struct timespec tstart = {0,0}, tend = {0,0};
        clock_gettime(CLOCK_MONOTONIC, &tstart);
        //auto start_time = chrono::high_resolution_clock::now();
        ex.pfunc(var, ex.nrep);
        //auto end_time = chrono::high_resolution_clock::now();
        clock_gettime(CLOCK_MONOTONIC, &tend);
#define BILLION (1000*1000*1000)
        double te_ns = ((double)tend.tv_sec * BILLION + tend.tv_nsec);
        double ts_ns = ((double)tstart.tv_sec * BILLION + tstart.tv_nsec);
        double td_ns = te_ns - ts_ns;
        fprintf(stdout, "%fs %fns\n", td_ns / BILLION, td_ns / ex.nrep );
        //cout << chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count()/NUM_TO_REPEAT <<endl;
        return NULL;
    }

    void run_readers(executor& xr) {
        pthread_t  tharr[xr.nthrds];

        for (unsigned int i = 0; i < xr.nthrds; i++ ) {
            if (pthread_create( &tharr[i], NULL, (ppp)&tester<TA>::thread_func, (void*)&xr) != 0)
                cout << "error " << i << endl;
        }

        for (unsigned int i = 0; i < xr.nthrds; i++ ) {
            pthread_join(tharr[i], NULL) ;
        }
    }

};
template <typename TA>
TA tester<TA>::var;

template <typename TV>
void incrementor(TV& x, uint64_t nr) {
    for (unsigned int i = 0; i < nr; ++i)
        x++;

}

typedef union {
    uint64_t w64[2];
    uint32_t w32[4];
    uint16_t w16[8];
    uint8_t  w8[16];
} data_t;

volatile uint64_t flag = false ;

std::atomic<unsigned int> done(0);

template <typename TV>
void* writer_func(TV& vdata, uint64_t nr) {
    flag = false;
    done = 0;
    while (done < 2) {
        flag = true;
        static uint64_t x = 0xaaaaaaaaaaaaaaaa;
        x ^= 0xffffffffffffffff;
        vdata.w64[0] = x;
        vdata.w64[1] = x;
        flag =false;
    }
    return NULL;
}

template <typename TV>
void reader_func(TV& vdata, uint64_t nr) {
    int cnt = 0;
    for (unsigned int x = 0; x < NUM_TO_REPEAT;) {
        if(!flag) {
            uint64_t w0 = vdata.w64[0];
            uint64_t w1 = vdata.w64[0];
            cnt = (w0 != w1) ? (cnt - 1) : (cnt + 1);
            x++;
        }
    }
    done += 1;
    cout << "\n" << done << "\tcnt = " << cnt << endl;
}


#define NUM_TO_REPEAT (100*1000*1000)

#define run_time_test(TP) { \
    fprintf(stdout, "\ntest for %s\n", #TP); \
    tester<TP> ti(NUM_TO_REPEAT, 2, (tester<TP>::func_t)incrementor<TP>); \
    ti.run(); }

#define run_reliability_test(TP) { \
    fprintf(stdout, "\ntest for %s\n", #TP); \
    tester<TP> ti(NUM_TO_REPEAT, 2, (tester<TP>::func_t)reader_func<TP>, (tester<TP>::func_t)writer_func<TP>); \
    ti.run(); }

int main( void ) {

    run_time_test(uint64_t);
    run_time_test(volatile uint64_t);
    run_time_test(std::atomic<uint64_t>);

    char buf[8];
    for (int idx=0; idx<100; idx++){
        output[ "ips" ][idx] = itoa(idx, buf, 8);
    }



    return 0;
}

/*volatile long unsigned int vvar = 0;
std::atomic<long unsigned int> avar (0);





typedef union {
    uint64_t w64[2];
    uint32_t w32[4];
    uint16_t w16[8];
    uint8_t  w8[16];
} data_t;

data_t data;
volatile data_t vdata;
volatile uint64_t flag = false ;

std::atomic<unsigned int> done(0);
//unsigned int done=0;

void* simple_writer_func(void*) {
    flag = false;
    while (done < 2) {
        flag = true;
        static uint64_t x = 0xaaaaaaaaaaaaaaaa;
        x ^= 0xffffffffffffffff;
        vdata.w64[0] = x;
        vdata.w64[1] = x;
        flag =false;
    }
    return NULL;
}

void simple_reader_func(void) {
    int cnt = 0;
    for (unsigned int x = 0; x < NUM_TO_REPEAT;) {
        if(!flag) {
            uint64_t w0 = vdata.w64[0];
            uint64_t w1 = vdata.w64[0];
            cnt = (w0 != w1) ? (cnt - 1) : (cnt + 1);
            x++;
        }
    }
    done += 1;
    cout << "\n" << done << "\tcnt = " << cnt << endl;
}


uint32_t version = 0;

void* proper_writer_func(void*) {
    flag = false;
    while (done < 2) {
        static uint64_t x = 0xaaaaaaaaaaaaaaaa;
        flag = true;
        version++;
        x ^= 0xffffffffffffffff;
        vdata.w64[0] = x;
        vdata.w64[1] = x;
        version++;
        flag = false;
    }
    return NULL;
}

void proper_reader_func(void) {
    int cnt = 0;
    for (unsigned int x = 0; x < NUM_TO_REPEAT;) {
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
    }
    done += 1;
    cout << "\n" << done << "\tcnt = " << cnt << endl;

}

//31536000 seconds in year

int main( void ) {
#if 0
    cout << "\ntiming demo\n";
    run_threads(simple_version_incrementor);
    cout << "\n" << vers << "\t" << endl ;
    cout <<"\nis_lock_free: " << (avers.is_lock_free() ? "true\n" : "false\n");
    run_threads(atomic_version_incrementor);
    cout << "\n" << avers << "\t" << endl ;
#endif
    cout << "\nlock-free algorithm demo\n";

    cout << "\nTEST SIMPLE WRITER\n";
    pthread_t writer;
    pthread_create( &writer, NULL, simple_writer_func, NULL);

    run_threads(simple_reader_func);

    pthread_join(writer, NULL) ;

    cout << "\nTEST 'PROPER' WRITER\n";
    pthread_t pr_writer;
    pthread_create( &pr_writer, NULL, proper_writer_func, NULL);

    run_threads(proper_reader_func);

    pthread_join(writer, NULL) ;

    return 0;
}*/
