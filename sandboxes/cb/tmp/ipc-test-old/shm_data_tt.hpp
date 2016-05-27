#ifndef SHM_DATA_TT_HPP_INCLUDED
#define SHM_DATA_TT_HPP_INCLUDED

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

template <typename TD, typename TV, typename TF, size_t MAX_ID, size_t MAX_IDX>
class shm_arr_hholder {

    template <typename TItem, size_t MAX_ITEM>
    class arr_wrap {
        typedef TItem TArr[MAX_ITEM];
        TArr arr;

    public:
        arr_wrap(TArr& rarr): arr(rarr) {}
        const TItem& operator[](size_t id) const {
            return arr[id];
        };
    };
public:
    typedef arr_wrap<TD, MAX_IDX> vals_arr_t;
    typedef arr_wrap<TV, MAX_IDX> vers_arr_t;
    typedef arr_wrap<TF, MAX_IDX> flag_arr_t;

    typedef arr_wrap<vap_vals_arr_t, MAX_ID> all_vals_t;
    typedef arr_wrap<vap_vers_arr_t, MAX_ID> all_vers_t;
    typedef arr_wrap<vap_flag_arr_t, MAX_ID> all_flag_t;

    //typedef TD all_data_arr_t[MAX_ID][MAX_IDX];
    //typedef TF all_flag_arr_t[MAX_ID][MAX_IDX];
    template <TTable>
    class shm_arr {
        struct SHMObj shmo;
        shm_arr(const char* prefix, const char* suffix, int flags=O_RDONLY ) {
            enum {MAX_NAME_LENGTH=256};
            char fname[MAX_NAME_LENGTH];

            if (!make_full_name(fname, MAX_NAME_LENGTH, prefix, suffix ) ) return;
            int ret = SHMOpenObj( fname, flags, sizeof(TTable), &shmo);//&shm_arr_holder::shmo_data_arr );
            if ( ret <= 0 ) {
                LOG( LOG_ERROR, "Couldn't open array as shared memory object: %s: %d",
                     fname, ret );
                return;
            }

        virtual ~shm_arr() {
            SHMCloseObj(&shmo);

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
            return true;
        }
    }

    class shm_arrays {
        shm_arr<all_vals_t> vals;
        shm_arr<all_vers_t> vers;
        shm_arr<all_flag_t> flgs;

    public:
        shm_arrays(const char* suffix, int flags=O_RDONLY ) :
            vals("vals-",suffix,flags),
            vers("vers-",suffix,flags),
            flgs("flgs-",suffix,flags|O_RDWR) ){
            LOG( LOG_DEBUG, "size_t all %d \tvap %d\n", sizeof(all_vals_t), sizeof(vals_arr_t) );
            LOG( LOG_DEBUG, "size_t all %d \tvap %d\n", sizeof(all_vers_t), sizeof(vers_arr_t) );
        }

        virtual ~shm_arrays() {
        }

    };

    class shm_data_reader   {

    public:
        shm_data_reader(const char* suffix):
            shm_arr_holder(suffix, O_RDONLY ) {}

        TD value(size_t id, size_t n) const {
            return (reinterpret_cast<vap_data_arr_t*>(shmo_data_arr.mmapPtr)[id])[n];
        };

        TD check_or_get_new_ip(TD& cur_data, back_end_server_ID,  num)
    if NOT bad_IP_flag_non_atomic_in_shmem
                if current_ip == shmem_array_of_IP_non_atomic[back_end_server_ID][num] // maybe need copy but I think it's not necessary
            return current_ip
    return get_valid_ip(back_end_server_ID)

    };

public:

    shm_arr_holder(const char* suffix, int flags=O_RDONLY ): pArrH(nullptr) {
    }


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
