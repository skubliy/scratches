#ifndef __IP_WRAP_HPP_
#define __IP_WRAP_HPP_

#include <arpa/inet.h>
#include <string.h>


//typedef  uint64_t dbl_word_t[2];
typedef union {
    struct in6_addr ip6;
    uint64_t w64[2];
    uint32_t w32[4];
} uip_t;

class ip_wrap {
    uip_t uip;

public:
/*
    ip_wrap() {
        uip.w64[0] = 0;
        uip.w64[1] = 0;
    }
*/
    ip_wrap(uint64_t w0=0, uint64_t w1=0) {
        uip.w64[0] = w0;
        uip.w64[1] = w1;
    }

    ip_wrap(const std::string& sip) {
        _str_to_ip(sip);
    }

    ip_wrap(const char* sip) {
        _str_to_ip(sip);
    }

    bool is_ipv4() const {
        return IN6_IS_ADDR_V4COMPAT(((const struct in6_addr *)&uip.ip6)) ? true : false;
    }

    bool is_ipv6() const {
        return !is_ipv4();
    }

    uip_t get_uip() const {
        return uip;
    }

    bool is_zero() const {
        return ( uip.w64[0] == 0 && uip.w64[1] == 0 ) ;
    }

    operator std::string () const {
        //TODO in future nee to do optimization here
        char buf6[INET6_ADDRSTRLEN];
        memset(buf6,0,sizeof(buf6));
        if ( IN6_IS_ADDR_V4COMPAT((const struct in6_addr *)&(uip.ip6))) {
            //TODO check portability
            inet_ntop(AF_INET, &(uip.ip6.s6_addr32[3]), buf6, INET_ADDRSTRLEN);
        } else {
            inet_ntop(AF_INET6, &uip.ip6, buf6, INET6_ADDRSTRLEN);
        }
        std::string sip(buf6);
        return sip;
    }

    ip_wrap& operator = (const std::string& sip) {
        _str_to_ip(sip);
        return *this;
    }

    ip_wrap& operator = (const ip_wrap& rhs) {
        const uip_t _uip = rhs.get_uip();
        uip.w64[0] = _uip.w64[0];
        uip.w64[1] = _uip.w64[1];
        return *this;
    }

    bool operator==(const ip_wrap& rhs) const {
        const uip_t _uip = rhs.get_uip();
        return ( uip.w64[0]==_uip.w64[0] && uip.w64[1]==_uip.w64[1] );
    }



    /*TODO
    ip_wrap& operator = (const struct in6_addr& _ip) {
        BLA-BLA-BLA
        return *this;
    }

    ip_wrap& operator = (const struct in_addr& _ip) {
        BLA-BLA-BLA
        return *this;
    }

    BLA-BLA-BLA
    */

private:
    int _str_to_ipv4(std::string sip) {
        for (int i=0; i<3; i++) uip.ip6.s6_addr32[i] = 0;
        return inet_pton(AF_INET, sip.c_str(), &(uip.ip6.s6_addr32[3]));
    }

    int _str_to_ipv6(std::string sip) {
        return inet_pton(AF_INET6, sip.c_str(), &uip.ip6);
    }

    int _str_to_ip(std::string sip) {
        return (sip.find(':') != std::string::npos) ? _str_to_ipv6(sip)
               : _str_to_ipv4(sip);
    }
};


#endif
