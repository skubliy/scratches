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

        long long int cnt = 0;
        uint64_t rcnt = 0;
        for (uint64_t i=0; i< num_repeat;) {

            ++rcnt;

            static shm_arr_ref_t shm_reader = _2darr_t::instance("test-IPs", O_RDONLY);
            ip_wrap ip(((shm_reader_t)shm_reader[0]).value_by_index(0));

            if (ip.is_invalid()) {
                continue;
            }

            ++i;

            uip_t uip = ip.get_uip();

            uint64_t w0 = uip.w64[0];
            uint64_t w1 = uip.w64[1];

            //if (w0 != w1) std::cout << "\n"<< std::hex << w0 <<"\t" << w1;

            cnt = (w0 != w1) ? (cnt - 1) : (cnt + 1);
        }

        //auto end_time = chrono::high_resolution_clock::now();
        clock_gettime(CLOCK_MONOTONIC, &tend);
        double te_ns = ((double)tend.tv_sec * BILLION + tend.tv_nsec);
        double ts_ns = ((double)tstart.tv_sec * BILLION + tstart.tv_nsec);
        std::cout
            << "\nrepeated " << num_repeat
            << "\trcnt = " << rcnt
            << "\tcnt = " << cnt
            << "\t" << (( te_ns - ts_ns ) / num_repeat )
            << " ns\n";
    }
    return 0;
}
