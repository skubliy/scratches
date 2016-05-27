
#include <fcntl.h> //for O_RDONLY


#ifndef LOG
#define LOG sprintf
#define LOG_DEBUG stdout
#define LOG_DEBUG2 stdout
#define LOG_ERROR stderr
#endif

//#include "shm_ip_arr.h"
#include <list>

#include "ip_wrap.hpp"

#define MAX_VAP 64
#define MAX_IP_PER_VAP 8

typedef std::list<ip_wrap*> ip_list_t;

typedef shm_ip_wrap vap_ip_arr_t [MAX_IP_PER_VAP];
typedef vap_ip_arr_t all_ip_arr_t[MAX_VAP];

typedef int vap_bad_ip_flags_t[MAX_IP_PER_VAP];
typedef vap_bad_ip_flags_t all_bad_ip_flags_t[MAX_VAP];

vap_ip_arr_t& get_ip_array( int sid );
vap_bad_ip_flags_t& get_bad_ip_flags_array( int sid );


#include "SHMObj.hpp"



#define SHMEM_IP_ARRAY_NAME "ip_array"
static struct SHMObj shmo_ip_arr;

#define SHMEM_BAD_IP_FLAGS_NAME "bad_ip_flags"
static struct SHMObj shmo_bad_ip_flags;

template <typename TArr, typename TItem>
class rarr_wrap{
    TArr& rarr;
public:
    rarr_wrap(TArr& ra): rarr(ra){}

    static TItem& operator[](size_t id)       {
        return rarr[id];
    };
    const TItem& operator[](size_t id) const {
        return rarr[id];
    };
};

template <typename TArr, typename TItem>
class arr_wrap{
    TArr arr;
public:
    //arr_wrap(TArr& ra): arr(ra){}
    // need redefine placement new operator
    static TItem& operator[](size_t id)       {
        return arr[id];
    };
    const TItem& operator[](size_t id) const {
        return arr[id];
    };
};


typedef rarr_wrap<all_ip_arr_t, vap_ip_arr_t> allIPs_t;
typedef rarr_wrap<vap_ip_arr_t, shm_ip_wrap> vapIPs_t;

typedef rarr_wrap<vap_bad_ip_flags_t, all_bad_ip_flags_t> allBIPF_t;
typedef rarr_wrap<vap_bad_ip_flags_t, int> vapBIPF_t;



vap_ip_arr_t* get_ip_array( int sid ){
    if (shmo_ip_arr.mmapPtr != nullptr)
        return allIPs_t[sid];
};
vap_bad_ip_flags_t& get_bad_ip_flags_array( int sid ){


};

bool ShMemIPArrCreate(){
    // Initialize IP array shared memory
    int ret = SHMOpenObj( SHMEM_IP_ARRAY_NAME,
                      O_CREAT | O_RDWR,
                      sizeof( all_ip_arr_t),
                      &shmo_ip_arr );
    if ( ret > 0 ) {
        LOG( LOG_DEBUG2, "Initialized IP array shared memory object." );
        SHMCloseObj( &shmo_ip_arr );
    } else {
        LOG( LOG_ERROR, "Could not open IP array shared memory object: %s: %d",
                SHMEM_IP_ARRAY_NAME, ret );
        return false;
    }

    int ret = SHMOpenObj( SHMEM_BAD_IP_FLAGS_NAME,
                      O_CREAT | O_RDWR,
                      sizeof( all_bad_ip_flags_t ),
                      &shmo_bad_ip_flags );
    if ( ret > 0 ) {
        LOG( LOG_DEBUG2, "Initialized bad IP flags array  shared memory object." );
        SHMCloseObj( &shmo_bad_ip_flags );
    } else {
        LOG( LOG_ERROR, "Could not open bad IP flags array shared memory object: %s: %d",
                SHMEM_BAD_IP_FLAGS_NAME, ret );
        return false;
    }

    // Initialize the Last Resource Expiry Date shared memory
    // O_EXCL is used to indicate that reinitializing the memory
    // should only occur if it doesn't already exist. We need this
    // timestamp persistent across restarts.

    //ret = SHMOpenObj( SOME_NAME, O_CREAT | O_EXCL | O_RDWR, sizeof( data ), &struct_SHMObj_INFO_HOLDER );
    return true;
};
