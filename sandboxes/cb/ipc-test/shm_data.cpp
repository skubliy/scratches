
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
#include "ip_wrap.hpp"
#include "shm_data_tt.hpp"

//typedef shm_flag<int> tf;

using namespace std;


typedef shm_arr_hholder<long, long, long, MAX_VAP, MAX_IP_PER_VAP> shmarr_t;

typedef shm_arr_hholder<ip_wrap, long, long, MAX_VAP, MAX_IP_PER_VAP> shm_ip_arr_t;

typedef int ai64[64];


static shm_ip_arr_t OWNER("xxIPs",O_RDWR|O_CREAT);
static shm_ip_arr_t READER("xxIPs",O_RDONLY);

ip_wrap check_or_get_new_val(ip_wrap& cur_ip, size_t id, size_t n){
    shm_ip_arr_t::shm_arrays& arr=reinterpret_cast<shm_ip_arr_t::shm_arrays&>(*READER.pInstance);
    shm_ip_arr_t::shm_data_reader dr(arr[id]);
    return  dr.check_or_get_new_val(cur_ip, n);
}

/*
int main(int argc, char* argv[]) {
    std::string str;
    {
        shm_obj<ai64> xxx("-1-","xxx",O_RDONLY);
        ai64& yyy=xxx;
        cout << "\n ------- reading \n";
        for(int i=0; i<64; i++)
            cout << yyy[i] << "\t";
        cout << endl;
        cin >> str;
        cout << "\n ------ writing \n";
        for(int i=0; i<64; i++)
            yyy[i]=i;
        cout << endl;
        cin >> str;
    }
    cout << endl;
    cin >> str;
    return 0;

    {
        shmarr_t* pshmHO= new shmarr_t("IPs", O_RDWR|O_CREAT);
        shmarr_t::shm_arrays* po = pshmHO->pInstance;
        shmarr_t::all_vals_t& pALL=po->vals;

        for (int i=0; i<4; i++) {
            shmarr_t::vals_arr_t& vals=pALL[i];//(shmarr_t::vals_arr_t&)(pALL[i]);
            for (int j=0; j<4; j++) {
                vals[j]=i*10+j;
            };
        };
        cout <<  endl;

        shmarr_t* pshmHR= new shmarr_t("IPs", O_RDONLY);
        pALL = pshmHR->pInstance->vals;
        for (int i=0; i<4; i++) {
            shmarr_t::vals_arr_t& vals =pALL[i];
            for (int j=0; j<4; j++) {
                long& x= pALL[i][j];
                cout << x<< "\t";
            };
            cout << endl;
        };

        pALL[1][1]=199;

        shmarr_t* pshmHW= new shmarr_t("IPs", O_RDWR);
        shmarr_t* pshmHR2= new shmarr_t("IPs", O_RDONLY);
        delete (pshmHR);
        delete (pshmHW);
        shmarr_t* pshmO2= new shmarr_t("IPs", O_RDWR|O_CREAT);
        cout <<  endl;
        cin >> str;
        delete (pshmO2);
        cout <<  endl;
        cin >> str;
        delete (pshmHO);
        cout <<  endl;
        cin >> str;
        delete (pshmHR2);

    }
    //tf flagi(0);
    //tf flagb(false);

    cout << endl;
    cin >> str;
    cout  << "\nThe end!\n" << str.c_str() << endl;
    return 0;
}
//const char const * shmem_file::root_path="/dev/shm/";

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
