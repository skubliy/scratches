#ifndef SHM_IP_ARR_H_
#define SHM_IP_ARR_H_

#include <list>

#include "ip_wrap.hpp"

//#define MAX_VAP 64
//#define MAX_IP_PER_VAP 8

typedef std::list<ip_wrap*> ip_list_t;

typedef shm_ip_wrap vap_ip_arr_t [MAX_IP_PER_VAP];
typedef vap_ip_arr_t all_ip_arr_t[MAX_VAP];

typedef int vap_bad_ip_flags_t[MAX_IP_PER_VAP];
typedef vap_bad_ip_flags_t all_bad_ip_flags_t[MAX_VAP];

vap_ip_arr_t& get_ip_array( int sid );
vap_bad_ip_flags_t& get_bad_ip_flags_array( int sid );

#endif // SHM_ARR_H_
