#ifndef __SHM_TABLE_HPP_
#define __SHM_TABLE_HPP_
#include <fcntl.h> //for O_RDONLY
#include <string.h> //for strlen
/*
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

*/

#include "SHMObj.hpp"

template <typename TD, typename TF, size_t MAX_ID, size_t MAX_IDX>
class shm_arr_hholder {

    template <typename TItem, size_t MAX_ITEM>
    class arr_wrap {
        typedef TItem TArr[MAX_ITEM];
        TArr& arr;

    public:
        arr_wrap(TArr& rarr): arr(rarr) {}
        const TItem& operator[](size_t id) const {
            return arr[id];
        };
    };
public:
    typedef arr_wrap<TD, MAX_IDX> vap_data_arr_t;
    typedef arr_wrap<TF, MAX_IDX> vap_flag_arr_t;

    typedef arr_wrap<vap_data_arr_t, MAX_ID> all_data_arr_t;
    typedef arr_wrap<vap_flag_arr_t, MAX_ID> all_flag_arr_t;

    //typedef TD all_data_arr_t[MAX_ID][MAX_IDX];
    //typedef TF all_flag_arr_t[MAX_ID][MAX_IDX];

public:
    struct SHMObj shmo_data_arr;
    struct SHMObj shmo_flag_arr;

    class shm_arr_holder {
    shm_arr_holder(const char* suffix, int flags=O_RDONLY ) {
        enum {MAX_NAME_LENGTH=256};
        char fname[MAX_NAME_LENGTH];

        LOG( LOG_DEBUG, "size_t all %d \tvap %d\n", sizeof(all_data_arr_t), sizeof(vap_data_arr_t) );

        if (!make_full_name(fname, MAX_NAME_LENGTH, "vals-", suffix ) ) return;
        int ret = SHMOpenObj( fname, flags, sizeof(all_data_arr_t), &shmo_data_arr);//&shm_arr_holder::shmo_data_arr );
        if ( ret <= 0 ) {
            LOG( LOG_ERROR, "Could not open data array as shared memory object: %s: %d",
                 fname, ret );
            return;
        }

        if (!make_full_name(fname, MAX_NAME_LENGTH, "flgs-", suffix ) ) return;
        ret = SHMOpenObj( fname, flags | O_RDWR, sizeof( all_flag_arr_t), &shmo_flag_arr);//&shm_arr_holder::shmo_flag_arr );
        if ( ret <= 0 ) {
            LOG( LOG_ERROR, "Could not open flags array as shared memory object: %s: %d",
                 fname, ret );
            return;
        }
    }

    virtual ~shm_arr_holder(){
        SHMCloseObj(&shmo_data_arr);
        SHMCloseObj(&shmo_flag_arr);
    }

    const vap_data_arr_t& operator[](size_t id) const {
            return reinterpret_cast<vap_data_arr_t*>(shmo_data_arr.mmapPtr)[id];
        };

private:
    bool make_full_name(char* buf, size_t max_length, const char* p, const char* s ) {
        static const char* prefix =  "arr-";

        if ( (strlen( prefix ) + strlen( p ) + strlen( s ) + 1 ) > max_length ) {
            LOG( LOG_ERROR, "Too long name" );
            //sprintf(stdout,)
            return false;
        }

        memset(buf, 0, max_length);
        sprintf( buf, "%s%s%s",prefix,p,s);
        //sprintf( buf, "%s%s", p,s);
        return true;
    }
    };

};

/*
template <typename TD, typename TF, size_t MAX_ID, size_t MAX_IDX>
struct SHMObj shm_arr_holder<TD, TF, MAX_ID, MAX_IDX>::shmo_data_arr;

template <typename TD, typename TF, size_t MAX_ID, size_t MAX_IDX>
struct SHMObj shm_arr_holder<TD, TF, MAX_ID, MAX_IDX>::shmo_flag_arr;
*/

template <typename TD, typename TF, size_t MAX_ID, size_t MAX_IDX>
class shm_data_reader : public shm_arr_holder<int, int, MAX_VAP, MAX_IP_PER_VAP> {

public:
    shm_data_reader(const char* suffix):
        shm_arr_holder(suffix, O_RDONLY ){}

        const vap_data_arr_t& get_vap_data(size_t id) const {
            return reinterpret_cast<vap_data_arr_t*>(shmo_data_arr.mmapPtr)[id];
        };

};

/*
template <typename TD, typename TF>
class shm_data_writer {

    shm_data<TD, TF>& shd;
public:
    shm_data_writer(size_t id, size_t n) {

    }
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
*/
#endif
