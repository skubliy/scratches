#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <list>


#ifndef LOG
#define LOG fprintf
#define LOG_DEBUG stdout
#define LOG_ERROR stderr
#endif

#ifndef nullptr
#define nullptr NULL
#endif

#define MAX_VAP 64
#define MAX_IP_PER_VAP 8





//#include "shm_ip_arr.h"
//#include "shm_data.hpp"
//#include "ip_wrap.hpp"
#include "shm_data_tt.hpp"

//typedef shm_flag<int> tf;

using namespace std;


//template <typedef TData, typename TTable= >

//shm_table_writer<int_arr_64_8<int> arr;

int main(int argc, char* argv[]) {
    //tf flag;
    {
    shm_arr_holder<long, long, MAX_VAP, MAX_IP_PER_VAP> shmH("IPs", O_RDWR);
    shm_data_reader<long, long, MAX_VAP, MAX_IP_PER_VAP> shmHr("IPs");
    shm_arr_holder<long, long, MAX_VAP, MAX_IP_PER_VAP>::vap_data_arr_t& rvarr=shmHr.get_vap_data(1);
    shm_arr_holder::vap_data_arr_t* pvarr=rvarr;
    fprintf(stdout, "\nsizeof(%d) sizeof(int)=%d %p %p\n", sizeof(shmH),sizeof(int),rvarr.pvarr);
    }
    //tf flagi(0);
    //tf flagb(false);
    std::string str;
    cout << endl;
    cin >> str;
    cout  << "\nThe end!\n" << str.c_str() << endl;
    return 0;
}
//const char const * shmem_file::root_path="/dev/shm/";
/*
int main(int argc, char* argv[]) {
    fprintf(stdout,"start\n");
    // if (argc <= 1)
    //     fprintf(stdout,"Usage:\txxx w 0 1\n or:\txxx r 0");
    // return -1;

    shm_obj<orig_ip_arr_t>* pshm;

    if (std::string(argv[1])=="w") {
        fprintf(stdout,"start1\n");
        pshm = new shm_orig_ip_holder();

    } else if (std::string(argv[1])=="r") {
        fprintf(stdout,"start2\n");
        pshm = new shm_orig_ip_consumer();
    }
    fprintf(stdout,"start3\n");
    // pshm = new shm_orig_ip_holder();
    fprintf(stdout,"start4\n");
    orig_ip_arr_t* parr=pshm->get_pshmt();
    orig_ip_arr_t& oiparr = *parr;
    fprintf(stdout,"start5\n");
    for (uint64_t i=0; i<64; i++) {
        for (uint64_t j=0; j<MAX_IP_PER_VAP; j++) {
            new(&(oiparr[i][j])) ip_wrap(i,j) ;

            std::string str=(std::string)(oiparr[i][j]);
            fprintf(stdout, "%s\n", str.c_str());
        }
        //fprintf(stdout, )
    }
    //shm_obj<orig_ip_arr_t>* pipw=new shm_obj<orig_ip_arr_t>();
    //orig_ip_arr_t* ptip2=pipw->open();


    fprintf(stdout,"%p\n%p\t%p\n%p\t%d\n"
            , &parr
            , parr,  parr[0]
            , &parr[0], parr[1]-parr[0]
           );
    cout << "The end!" << endl;
    return 0;
}*/
