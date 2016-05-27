#ifndef SHM_DATA_TT_HPP_INCLUDED
#define SHM_DATA_TT_HPP_INCLUDED

//#include <sys/mman.h>   // shm_open
//#include <sys/stat.h>   // fstat, stat ...
//#include <unistd.h>     // ftruncate
//#include <fcntl.h>      // definition of O_XX constants: O_RDONLY, O_RDWR, O_CREATE ...
//#include <string.h>     // strlen
//#include <errno.h>      // errno

#include "shm_obj.hpp"

#define time_not_expaired 1 //TODO zzz

template <typename TD, typename TV, typename TF, size_t MAX_ID, size_t MAX_IDX>
class shm_arr_hholder {

    template <typename _TI, size_t _MAX_ITEM>
    class arr_wrap {
        typedef arr_wrap<_TI, _MAX_ITEM> __Tslf;
        typedef _TI*    __TIptr;
        typedef _TI&    __TIref;
        typedef _TI     __TIarr[_MAX_ITEM];

        __TIarr _arr;

    public:
        //TODO public for now
        __TIref operator[](size_t id) {
            LOG( LOG_DEBUG, "operator %d sz=%d\n", id, sizeof(__TIarr) );
            return _arr[id];
        }
        //TODO it is public for now
        operator __TIref() {
            return (_arr);
        }

        operator __TIptr() {
            return (&_arr[0]);
        }
    };

public:
    typedef arr_wrap<TV, MAX_IDX> vers_arr_t;
    typedef arr_wrap<TD, MAX_IDX> vals_arr_t;
    typedef arr_wrap<TF, MAX_IDX> flgs_arr_t;

    typedef arr_wrap<vers_arr_t, MAX_ID> all_vers_t;
    typedef arr_wrap<vals_arr_t, MAX_ID> all_vals_t;
    typedef arr_wrap<flgs_arr_t, MAX_ID> all_flgs_t;

private:


    template <typename _TI, size_t _MAX_ITEM>
    class rarr_rdo {
        typedef _TI     __TIarr[_MAX_ITEM];
        __TIarr& _rarr;

    public:
          _TI operator[](size_t id) const {
            LOG( LOG_DEBUG, "zzz rdo operator[] const %d \n", id );
            return _rarr[id];
        }
    };

public:
        typedef rarr_rdo<TV, MAX_IDX> rvers_t;
        typedef rarr_rdo<TD, MAX_IDX> rvals_t;
        typedef rarr_rdo<TF, MAX_IDX> rflgs_t;

    class shm_refs_arr {

    public:
        vers_arr_t& rvers;
        vals_arr_t& rvals;
        flgs_arr_t& rflgs;

        shm_refs_arr(rvers_t _rvers, rvers_t _rvals, rvers_t _rflgs) :
            rvers(_rvers), rvals(_rvals), rflgs(_rflgs)  {
        }

        shm_refs_arr(shm_refs_arr& _ra) :
            rvers(_ra.rvers), rvals(_ra.rvals), rflgs(_ra.rflgs)  {
        }

    };

    class shm_arrays {

    public:
        shm_obj<all_vers_t> vers;
        shm_obj<all_vals_t> vals;
        shm_obj<all_flgs_t> flgs;

        shm_arrays(const char* suffix, int flags=O_RDONLY ) :
            vers( "vers-", suffix, flags ),
            vals( "vals-", suffix, flags ),
            flgs( "flgs-", suffix, flags | O_RDWR )  {
        }

        virtual ~shm_arrays() {
        }

        const shm_refs_arr& operator[](size_t id) const {
            shm_refs_arr _trarr(vers[id], vals[id], flgs[id]);
            return _trarr;
        }

    };

    class shm_data_reader  {
        shm_refs_arr rarr;
    public:
        shm_data_reader(const shm_refs_arr& _rarr):
            rarr(_rarr) {}
    private:
        //TD value(size_t id, size_t n) const {
        //    return (reinterpret_cast<vap_data_arr_t*>(shmo_data_arr.mmapPtr)[id])[n];
        //};
        TD get_valid_data_by_num(size_t n) {
            //TODO need check type of version_holder it is must beatomic for now,
            // may we do it non atomic, volotile for example?
            TV version_before = rarr.vers[n]; //it is atomic copy assignment
            if (version_before & 1) // If not even, the value in process of changing right now.
                return 0;

            TD data = rarr.vals[n];
            TF version_after = rarr.vers[n]; // it is atomic copy assignment

            if  (version_before != version_after)  // versions is not sames
                // we may add extra check of version evenest
                //OR  (version_after & 1) //or versions is not even value is not correct
                return 0;
            return data;
        }

        TD get_valid_val() {
            while(time_not_expaired) { //TODO zzz
                for (size_t n = 0; n < MAX_IDX; n++) {
                    TD data = get_valid_data_by_num(n);
                    if (data)
                        return data;
                }
                //"it will never happened" "if it happened and time expired do something
                //but it is problems on the back end servers site
                //it is similar situation with situation when back end server just down”
            }
            return 0; // or some server_down_ip ;)
        }

    public:
        TD check_or_get_new_val(TD& cur_val, size_t n) {
            if ( !(rarr.flgs[n]) )
                if (cur_val == rarr.vals[n]) // maybe need copy but I think it's not necessary
                    return cur_val;
            return get_valid_val();
        }

    };


    class shm_data_writer : public shm_arrays {
    public:
        shm_data_writer(const char* suffix, size_t flags = O_RDWR):
            shm_arrays(suffix, flags ) {}

        void set_value (const TD& new_val, size_t id, size_t n) {
            if ( this->flag[id][n]) {  // in real code this check will be not here but will have same sense
                this->vers[id][n] ++; //it is increment of atomic var version and version become NOT even
                this->vals[id][n] = new_val;   //it  copy assignment
                this->vers[id][n] ++; //it is increment of atomic var version and version become even
                this->flgs[id][n] = 0;
            }
            return *this;

        };

        virtual ~shm_data_writer() {
        }

        virtual const shm_refs_arr& operator[](size_t id) const {};
    };



public:
    //    shm_arr_hholder& instance()
private:

public:
    shm_arrays* pInstance;
    shm_arr_hholder(const char* suffix, int flags=O_RDONLY) : pInstance(nullptr) {
        if (flags | O_RDWR) {
            pInstance= new shm_data_writer(suffix, flags);
        } else if (flags == O_RDONLY) {

            pInstance= new shm_arrays(suffix, O_RDONLY);
        }
    }

    ~shm_arr_hholder() {
        if (pInstance) {
            delete(pInstance);
            pInstance = nullptr;
        }
    };
    /*


    class shm_data_reader1 : public shm_arrays {
    public:
        shm_data_reader1(const char* suffix):
            shm_arrays(suffix, O_RDONLY ) {}

    private:
        //TD value(size_t id, size_t n) const {
        //    return (reinterpret_cast<vap_data_arr_t*>(shmo_data_arr.mmapPtr)[id])[n];
        //};
        TD get_valid_data_by_num(size_t id, size_t n) {
            //TODO need check type of version_holder it is must beatomic for now,
            // may we do it non atomic, volotile for example?
            TV version_before = this -> vers[id][n]; //it is atomic copy assignment
            if (version_before & 1) // If not even, the value in process of changing right now.
                return 0;

            TD data = this->vals[id][n];
            TF version_after = this->vers[id][n]; // it is atomic copy assignment

            if  (version_before != version_after)  // versions is not sames
                // we may add extra check of version evenest
                //OR  (version_after & 1) //or versions is not even value is not correct
                return 0;
            return data;
        }

        TD get_valid_val(size_t id) {

            while(time_not_expaired) { //TODO zzz
                for (size_t n = 0; n < MAX_IDX; n++) {
                    TD data = get_valid_data_by_num(id, n);
                    if (data)
                        return data;
                }
                //"it will never happened" "if it happened and time expired do something
                //but it is problems on the back end servers site
                //it is similar situation with situation when back end server just down”
            }
            return 0; // or some server_down_ip ;)
        }

    public:
        TD check_or_get_new_val(TD& cur_val, size_t id, size_t n) {
            if ( !(this->flgs[id][n]) )
                if (cur_val == this->vals[id][n]) // maybe need copy but I think it's not necessary
                    return cur_val;
            return get_valid_val(id);
        }

    };


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

};

#endif
