#ifndef SHM_2DARR_COMMON_HPP_INCLUDED
#define SHM_2DARR_COMMON_HPP_INCLUDED

#ifndef LOG
#include <cstdio>
#   define LOG fprintf
#   define LOG_ERROR stderr
#   define LOG_DEBUG stdout
#   define LOG_INFO stdout
#endif // ifdef LOG end

#include <stdint.h>

#include "shm_obj.hpp"
#include "shm_2Darr.hpp"
#include "ip_wrap.hpp"

#define MAX_VAP 1
#define MAX_IP_PER_VAP 1

template <typename TV>
class cvers {
    TV vers;
public:
    cvers(TV v=0): vers(v) {}
    //cvers(cvers& cv) {}
    cvers& operator = (const TV& v) {
        vers = v;
        return *this;
    }

// prefix increment operator.
    cvers& operator ++ () {
        cvers<TV>::fetch_and_add(&vers,1);
        return *this;
    }

// postfix increment operator.
    cvers& operator ++ (int) {
        cvers temp = *this;
        ++*this;
        return temp;
    }

    operator TV() {
        return vers;
    }

    /*TV fetch_and_add(volatile TV* dest, TV increment) {
        TV result = increment;
        asm volatile ("lock xadd %[src], %[dest]"
                      : [dest] "+m" (*dest),
                      [src] "+r" (result)
                      :
                      : "memory", "cc");
        return result;
    }*/

      inline TV fetch_and_add( TV * variable, int value ) {
      asm volatile("lock; xaddl %%eax, %2;"
                   :"=a" (value)                  //Output
                   :"a" (value), "m" (*variable)  //Input
                   :"memory");
      return value;
  }
};


typedef uint32_t flag_t;

//typedef uint32_t version_t;
//typedef cvers<volatile uint32_t> version_t;
typedef volatile uint32_t version_t;

//#include <atomic>
//typedef std::atomic<uint32_t> version_t;


typedef shm_2Darr<version_t, ip_wrap, flag_t, MAX_VAP, MAX_IP_PER_VAP> _2darr_t;
typedef typename _2darr_t::shm_arrays& shm_arr_ref_t;

typedef typename _2darr_t::shm_data_writer shm_writer_t;

typedef typename _2darr_t::shm_data_reader shm_reader_t;

#endif //SHM_2DARR_COMMON_HPP_INCLUDED
