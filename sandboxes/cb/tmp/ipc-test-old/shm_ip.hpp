#ifndef __SHM_IP_HPP_
#define __SHM_IP_HPP_

//#define MAX_IP_PER_VAP 8

typedef ip_wrap ip_array_t[MAX_IP_PER_VAP];
typedef std::list<ip_wrap*> ip_list_t;
typedef ip_array_t orig_ip_arr_t[64];

typedef int bad_ip_flags_array_t [MAX_IP_PER_VAP];
typedef bad_ip_flags_array_t all_bad_ip_flags_t [64];

typedef std::atomic<int> ip_version_array_t [MAX_IP_PER_VAP];
typedef ip_version_array_t all_ip_version_array_t [64];

#define MAX_VAP 64

//ip_array_t& get_ip_array( int sid );
//bad_ip_flags_array_t& get_bad_ip_flags_array( int sid );




class shm_ip_arr{
public:
    ip_array_t& aip;
    ip_version_array_t& avip;
    bad_ip_flags_array_t& abipf;
};

class orig_ip_holder: : public shm_ip_arr {

public:
    static ip_wrap& operator[](size_t n)       {
        return aip[idx];
    };
    const ip_wrap& operator[](size_t n) const {
        return aip[n];
    };
private:
    void reassign_IP_by_num(ip_wrap& new_IP, size_t n) {
        if  abipf[n] {  // in real code this check will be not here but will have same sense
            avip[n] ++ //it is increment of atomic var version and version become NOT even
            aip[n] = new_IP   //it  copy assignment
            avip[n] ++ //it is increment of atomic var version and version become even
            abipf[n] = 0
        }
    };
};

class shm_ip_reader : public shm_ip_arr {

public:

    bool time_not_expired(){retutn true;};//TODO

    ip_wrap& get_valid_ip_by_num(num){
        //TODO  version_holder is atomic for now, may we do it non atomic? I think YES. Are any arguments in opposites?
        // Instead of atomic can we possible use volatile?
        uint64_t version_before = vip[num]; //it is atomic copy assignment
        if (version_befor & 1)    // is not even
                return 0;

        ip_wrap& ip = aip[num];
        uint64_t version_after = vip[num]; // it is atomic copy assignment

        if  (version_before != version_after)  // versions is not sames
                // we may add extra check of version evenest but it will cost us 1 slow atomic operation and excessively
                //OR NOT (version_after & 1) //or versions is not even ip is not correct
                return 0;
        return ip;
    }

    ip_wrap& get_valid_ip(){
        while(time_not_expaired) {
            for (n=0; n<16; n++){
                   ip_wrap& ip = get_valid_ip_by_num( n);
                   if (ip != 0)
                           return ip;
            }
     }
        return 0 // or some server_down_ip ;)
    }

ip_wrap&  check_or_get_new_ip( ip_wrap& curent_ip, size_t  num)
    if (abipf[n].isnot_set()){
                if current_ip == aip[num] // maybe need copy but I think it's not necessary
            return current_ip
            }
    return get_valid_ip(back_end_server_ID)
}




class shm_orig_ip_holder   {
    static shm_obj_writer<orig_ip_arr_t> aip("orig-ip-stor");
    static shm_obj_writer<all_bad_ip_flags_t> abipf("bad_ip_flags");

public:
    static orig_ip_per_vap_holder& get_by_id(size_t _id) {

    }

    static value_t& operator[](std::size_t id)       {
        return mVector[idx];
    };
    const value_t& operator[](std::size_t id) const {
        return mVector[idx];
    };
};

class shm_orig_ip_consumer : public shm_obj_reader<orig_ip_arr_t> {
public:
    shm_orig_ip_consumer() : shm_obj_reader<orig_ip_arr_t>("orig-ip-stor") {}
};

#endif
