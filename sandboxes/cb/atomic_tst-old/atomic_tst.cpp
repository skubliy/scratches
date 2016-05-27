extern "C"
{
#include <pthread.h>
}

#include <iostream>
#include <atomic>

using namespace std;

#define BILLION (1000*1000*1000)
typedef void (*fp_t)(void);
#define NUM_TO_REPEAT (1000*1000*1000)

template <typename TA>
class executor{
public:
    typedef void (*func_t)(TA&);
    static func_t func;
    static TA arg;
    static uint64_t nrepeat;
    executor(func_t f, TA& a, uint64_t nr = NUM_TO_REPEAT):
            func(f), arg(a), nrepeat(nr){}
}

template <typename TA>
void* thread_func(void* p) {
    executor<TA>& ex = *p;
    struct timespec tstart = {0,0}, tend = {0,0};

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    //auto start_time = chrono::high_resolution_clock::now();
    ex.func(ex.arg, ex.nrepeat);
    //auto end_time = chrono::high_resolution_clock::now();
    clock_gettime(CLOCK_MONOTONIC, &tend);
    double te_ns = ((double)tend.tv_sec * BILLION + tend.tv_nsec);
    double ts_ns = ((double)tstart.tv_sec * BILLION + tstart.tv_nsec);
    fprintf(stdout, "%f ns\n",(( te_ns - ts_ns ) / ex.nrepeat ));
    //cout << chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count()/NUM_TO_REPEAT <<endl;
    return NULL;
}

template <typedef TA>
void run_threads(executor<TA>* pex, uint32_t num_threads = 2) {
    pthread_t  tharr[num_threads];

    for (unsigned int i = 0; i < num_threads; i++ ) {
        if (pthread_create( &tharr[i], NULL, thread_func<TA>, (void*)pex) != 0)
            cout << "error " << i << endl;
    }

    for (unsigned int i = 0; i < num_threads; i++ ) {
        pthread_join(tharr[i], NULL) ;
    }
}

template <typename TV>
void incrementor(TV& x, uint64_t nr) {
    for (unsigned int i = 0; i < nr; ++i)
        x++;
}

template<typename TV>
class incr{
static TV var = 0;


}


volatile long unsigned int vvar = 0;
std::atomic<long unsigned int> avar (0);

executor<l>



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
}
