#ifndef __SHM_DATA_HPP_
#define __SHM_DATA_HPP_

#include "shm_table.hpp"

template <typename TF>
class shm_flag {
    #define MAX_ID 64
    #define MAX_N 8
    typedef TF flag_stor_t[MAX_ID][MAX_N];
    static shm_table_writer<flag_stor_t<TF>> g_flag_stor;
    static TF& flag_ref(size_t id, size_t n){
        return g_flag_stor[id][n];
    }

    TF& flag;
public:
    shm_flag(size_t id, size_t n):flag(&fl) {};


    operator bool() {
        fprintf(stdout,"bool was caled\n");
        return (flag == 0) ? false : true;
    }

    bool is_set() {
        return (bool)flag;
    }
    bool isnt_set() {
        return !is_set();
    }

    shm_flag& operator = (const TF fl) {
        flag = fl;
        return *this;
    }

    shm_flag& operator = (const TF& rfl) {
        flag = rfl;
        return *this;
    }
};

template <typename TD, typename TF>
class shm_data {
public:
    TD& val;
    int& ver;
    TF& bfl;
};

template <typename TD, typename TF>
class shm_data_writer {
    shm_data<TD, TF>& shd;
public:
    shm_data_writer& operator = (const TD& new_val) {
        if  (shd.bfl.is_set()) {  // in real code this check will be not here but will have same sense
            shd.ver ++; //it is increment of atomic var version and version become NOT even
            shd.val = new_val;   //it  copy assignment
            shd.ver ++; //it is increment of atomic var version and version become even
            shd.bfl = 0;
        }
        return *this;

    };
};

#endif
