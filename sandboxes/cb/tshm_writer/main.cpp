#include <iostream>

#include "shm_2Darr_common.hpp"

int main(int argc, char* argv[]){
#define BILLION (1000*1000*1000)
#define NUM_TO_REPEAT (10*1000*1000)
    uint64_t num_repeat = (argc > 1) ? (uint64_t)atol(argv[1]) : NUM_TO_REPEAT;

    while(1) {
        struct timespec tstart = {0,0}, tend = {0,0};
        clock_gettime(CLOCK_MONOTONIC, &tstart);
        //auto start_time = chrono::high_resolution_clock::now();
        for (uint64_t i=0; i< num_repeat; ++i) {

            static uint32_t x = 0xaaaaaaaa;
            x ^= 0xffffffff;
            ip_wrap v(x,x,x,x);

            static shm_arr_ref_t ref_writer = _2darr_t::instance("test-IPs", O_CREAT|O_RDWR);
            ((shm_writer_t)ref_writer[0]).set_val_by_idx(0,v);
        }

        //auto end_time = chrono::high_resolution_clock::now();
        clock_gettime(CLOCK_MONOTONIC, &tend);
        double te_ns = ((double)tend.tv_sec * BILLION + tend.tv_nsec);
        double ts_ns = ((double)tstart.tv_sec * BILLION + tstart.tv_nsec);
        std::cout
            << "\nrepeated "<< num_repeat
            << "\t" << (( te_ns - ts_ns ) / num_repeat )
            << " ns\n";

        sleep(10);

    }
    return 0;
}


