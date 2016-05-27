#ifndef SHM_2DARR_HPP_INCLUDED
#define SHM_2DARR_HPP_INCLUDED

//#include <sys/stat.h>   // fstat, stat ...
//#include <unistd.h>     // ftruncate
//#include <fcntl.h>      // definition of O_XX constants: O_RDONLY, O_RDWR, O_CREATE ...
//#include <string.h>     // strlen
//#include <errno.h>      // errno

#include <stdlib.h>     // srand, rand

#include <list>

#include "shm_obj.hpp"

#define time_not_expaired 1 //TODO need to release the timer

template <typename TV, typename TD, typename TF, uint32_t MAX_ID, uint32_t MAX_IDX>
class shm_2Darr {

public:
    //typedef std::atomic<TV> vers_arr_t[MAX_IDX];
    typedef volatile TV vers_arr_t[MAX_IDX];
    typedef TD vals_arr_t[MAX_IDX];
    typedef TF flgs_arr_t[MAX_IDX];

    typedef vers_arr_t all_vers_t[MAX_ID];
    typedef vals_arr_t all_vals_t[MAX_ID];
    typedef flgs_arr_t all_flgs_t[MAX_ID];

    class shm_refs_arr {

        typedef vers_arr_t& rvers_t;
        typedef vals_arr_t& rvals_t;
        typedef flgs_arr_t& rflgs_t;

    public:
        rvers_t rvers;
        rvals_t rvals;
        rflgs_t rflgs;

        shm_refs_arr(rvers_t _rvers, rvals_t _rvals, rflgs_t _rflgs) :
            rvers(_rvers), rvals(_rvals), rflgs(_rflgs)  {
        }

        shm_refs_arr(const shm_refs_arr& _ra) :
            rvers(_ra.rvers), rvals(_ra.rvals), rflgs(_ra.rflgs)  {
        }

    };

    class shm_arrays {

        shm_obj<all_vers_t> vers;
        shm_obj<all_vals_t> vals;
        shm_obj<all_flgs_t> flgs;
    public:
        shm_arrays(const char* suffix, int flags=O_RDONLY ) :
            // VIPlace where we creating/opening shmem files
            vers( "vers-", suffix, flags ),
            vals( "vals-", suffix, flags ),
            flgs( "flgs-", suffix, flags | O_RDWR )  {
        }

        virtual ~shm_arrays() {
        }

        const shm_refs_arr operator[](uint32_t id) const {
            //accert (id > 0 && id < MAX_ID);
            //LOG( LOG_DEBUG,  "shm_2Darr::shm_arrays.shm_refs_arr %d", id);
            shm_refs_arr _trarr(((all_vers_t&)vers)[id], ((all_vals_t&)vals)[id], ((all_flgs_t&)flgs)[id] );
            return _trarr;
        }
    };

private:

public:
    class shm_data_reader  {
    protected:
        shm_refs_arr rarr;
    public:
        shm_data_reader(const shm_refs_arr& _rarr):
            rarr(_rarr) {}

    public:

        void print_all_info(uint32_t idx) {
            TF x = rarr.rflgs[idx];
            LOG( LOG_DEBUG, "zzz-elb: idx=%d(%d) %d %s",
                 idx, x, rarr.rvers[idx], std::string(rarr.rvals[idx]).c_str());
        }

        TD value_by_index(uint32_t idx) const {
            if (idx >= MAX_IDX) {
                LOG( LOG_ERROR, "%s: was passed not valid index %d", __func__, idx);
                return zero();
            }

            if ( ! rarr.rflgs[idx] )
                return zero();

            TV version_before = rarr.rvers[idx]; // it is a copy assignment
            if (version_before & 1) // If not even, the value in process of changing right now.
                return zero();

            TD data = rarr.rvals[idx];

            asm volatile ("" ::: "memory"); // here enough just a compiler barrier
            //__sync_synchronize();

            TV version_after = rarr.rvers[idx]; // it is a copy assignment
            if  (version_before != version_after)  // versions is not same
                // we may add extra check of version parity
                //OR  (version_after & 1) //or versions is not even value is not correct
                return zero();
            return data;
        }

        TD get_any_valid_val(uint32_t& idx) const {
            LOG( LOG_DEBUG, "zzz-elb-trace-delme %s: %d", __func__, idx);
            for (int i = 0; i < 2; i++) {
                //TODO need release some waiting timer here
                //while(time_not_expaired) {
                if ( idx >= MAX_IDX) {
                    idx = 0; //TODO: get_random();
                }
                for (; idx < MAX_IDX; idx++) {
                    TD data(value_by_index(idx));
                    if ( data != zero()) {
                        LOG( LOG_DEBUG, "zzz-elb-delme %s: %d", __func__, idx);
                        return data;
                    }
                }
                idx = 0;
            }
            //TODO it is newer happened if so it is a problem
            // lets give chance to solve it on different level
            LOG( LOG_ERROR, "zzz-elb get_any_valid_val: coudn't find valid value");
            //FIXME delme
            for (; idx < MAX_IDX; idx++) {
                TD data(value_by_index(idx));
                if ( data != zero()) {
                    LOG( LOG_DEBUG, "zzz-elb-trace-delme %s: [%d](%d) %s",
                            __func__, idx, rarr.rflgs[idx], std::string(data).c_str());
                    return data;
                }
            } //FIXME end of delme
            return zero();
        }

        TD check_or_get_new_val(const TD& cur_val, uint32_t& idx) const {
            if (idx >= MAX_IDX) {
                LOG( LOG_INFO, "zzz-elb check_or_get_new_val: was passed not valid index %d", idx);
                idx = 0; //TODO: or may be something random?
            }

            if ( ! rarr.rflgs[idx] ) // goodness flag is reset =0 It is market as bad value
                return get_any_valid_val(idx);

            if ( cur_val == rarr.rvals[idx])
                return cur_val;

            return get_any_valid_val(idx);
        }

        bool reset_gflag(TD& bad_data, uint32_t idx) {
            uint32_t i = idx;

            TD data(get_any_valid_val(i));

            if ( data == bad_data && i == idx ) {
                rarr.rflgs[idx] = false;
                LOG( LOG_INFO, "%s: [%d] was invalidated", __func__, idx);
            } else {
                LOG( LOG_DEBUG, "%s:  nothing to reset value [%d] not in the table any more", __func__, idx);


                if ( data != zero()) {
                    LOG( LOG_DEBUG, "%s: new valid value was found in the table in [%d] new valid val in", __func__, idx);
                    return true;
                }
            }
            return false;
        }

    protected:
        uint32_t get_random() {
            return (rand() % MAX_IDX);
        }

        const TD& refzero() const {
            static const TD zero_data((uint32_t)0);
            return zero_data;
        }

        TD zero() const {
            static const TD zero_data((uint32_t)0);
            return zero_data;
        }

    };

    class shm_data_writer : public shm_data_reader {
        typedef std::list<TD> new_vals_list;
        typedef typename std::list<TD>::iterator new_vals_list_iter;
    public:
        shm_data_writer(const shm_refs_arr& arr):
            shm_data_reader(arr) {}

    private:
        void update_val(volatile TV& ver, TD& val, const TD nval){
            // ++ver;
            __sync_fetch_and_add(&ver, 1);
            val = nval;
            // ++ver;
            __sync_fetch_and_add(&ver, 1);
        }

    public:
        void invalidate_array(void){
            shm_refs_arr _rarr=this->rarr;

            for ( uint32_t idx = 0; idx < MAX_IDX; idx++ ) {
                _rarr.rflgs[idx] = false;
                if (_rarr.rvals[idx] != this->zero()){
                    update_val(this->rarr.rvers[idx], this->rarr.rvals[idx], this->zero());
                }
            }
        }

        void set_val_by_idx(uint32_t idx, const TD val){
            this->rarr.rflgs[idx] = false;
            update_val(this->rarr.rvers[idx], this->rarr.rvals[idx], val);
            this->rarr.rflgs[idx] = true;
        }

        void update_vals_array(new_vals_list& nwl) {
            shm_refs_arr _rarr=this->rarr;

            // Look for those IPs that have not changed
            // in 99.999% it will be all of them

            // lookup for values which not in the new list of
            LOG( LOG_DEBUG, "zzz-elb-dns update_vals_array: %lu values in new_vals list", nwl.size());
            for ( uint32_t idx = 0; idx < MAX_IDX; idx++ ) {
                TF gflg(false);
                for ( new_vals_list_iter itr = nwl.begin(); itr != nwl.end(); ) {
                    if (*itr == _rarr.rvals[idx]) {

                        //value is valid so reseting the bflag
                        gflg = true;
                        // erasing value from the new list
                        itr = nwl.erase( itr );
                        break;
                    } else {
                        ++itr;
                    }
                }

                _rarr.rflgs[idx] = gflg; // value not changed but may became bad or good
                if (!gflg && (_rarr.rvals[idx] != this->zero()) ) { //TODO not sure need to zeroed old IPs ???
                    LOG( LOG_DEBUG, "zzz-elb-dns was found not valid values it will be invalidated [%d]%s",
                         idx, std::string(_rarr.rvals[idx]).c_str());
                    update_val(this->rarr.rvers[idx], this->rarr.rvals[idx], this->zero());
                }

            }

            // Look for those IPs that wasn't in the previous list
            LOG( LOG_DEBUG, "zzz-elb-dns update_vals_array: %lu \"fresh\" values in new_vals list", nwl.size());
            for ( new_vals_list_iter itr = nwl.begin(); itr != nwl.end();) {
                TF gflg(false);
                uint32_t idx;
                for (idx=0; idx < MAX_IDX; idx++) {
                    if (( _rarr.rvals[idx] == this->zero() ) || (_rarr.rflgs[idx] == false)) {
                        LOG( LOG_DEBUG, "zzz-elb updateVapWithFqdn: updating value [%d] from %s to %s",
                             idx, std::string(_rarr.rvals[idx]).c_str(), std::string(*itr).c_str());
                        update_val(this->rarr.rvers[idx], this->rarr.rvals[idx], *itr);
                        _rarr.rflgs[idx] = gflg = true;

                        LOG( LOG_DEBUG, "zzz-elb-dns updateVapWithFqdn: checking value [%d](%d) from %s ",
                             idx, _rarr.rflgs[idx], std::string(_rarr.rvals[idx]).c_str() );
                        break;
                    }
                }

                if (gflg) {
                    itr = nwl.erase( itr );
                } else {
                    //TODO it is never happened?
                    LOG( LOG_ERROR, "zzz-elb-dns update_vals_array: ERROR ");
                    ++itr;
                }
            }

            // Checking if the new list still have values.
            // If so, we can treat it as: "wrong situation"
            if ( nwl.size() != 0 ) { //new_ip_list must be empty there
                //TODO it is not error but it is really strange case we have to increase size of values list
                LOG( LOG_ERROR, "zzz-elb-dns update_vals_array: ERROR: %lu values still in new_vals list", nwl.size());
                for (new_vals_list_iter itr = nwl.begin(); itr != nwl.end(); ) {
                    LOG( LOG_ERROR, "zzz-elb-dns update_vals_array: values = %s still in the list", std::string(*itr).c_str() );
                    itr = nwl.erase( itr );
                }
            }

        }

        bool check_gflags() const {
            shm_refs_arr _rarr=this->rarr;
            for ( uint32_t ii = 0; ii < MAX_IDX; ii++ ) {
                if (!_rarr.rflgs[ii] && (_rarr.rvals[ii] != this->zero()) ) {
                    static int x=0;
                    //FIXME it is hack for debug DO NOT FORGET TO REMOVE IT
                    if (x++ > 12) {
                        x=0;
                        return true;
                    }
                }
            }
            return false;
        }
    };

public:
    //TODO if read only need to check if shm_object is already initialized
    // for now it will be SEG
    static shm_arrays& instance(const char* const suffix, int flags=O_RDONLY) {
        static shm_arrays _instance(suffix, flags);
        return _instance;
    }

};

#endif
