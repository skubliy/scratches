#ifndef __IP_WRAP_HPP_INCLUDED
#define __IP_WRAP_HPP_INCLUDED

#ifndef __cplusplus
#error "Check your #include tree; you cannot include this from within C code"
#endif

#include <iostream>
#include <arpa/inet.h>
#include <string.h>

typedef union {
    struct in6_addr ip6;
    uint64_t w64[2];
    uint32_t w32[4];
    uint16_t w16[8];
    uint8_t  w8[16];
} uip_t;

class ip_wrap {
    uip_t uip;

public: // family of methods to work with ip_wrap itself

    ip_wrap(const ip_wrap& rhs) {
        const uip_t _uip = rhs.get_uip();
        uip.w64[0] = _uip.w64[0];
        uip.w64[1] = _uip.w64[1];
    }

    ip_wrap& operator = (const ip_wrap& rhs) {
        const uip_t _uip = rhs.get_uip();
        uip.w64[0] = _uip.w64[0];
        uip.w64[1] = _uip.w64[1];
        return *this;
    }

    bool operator != (const ip_wrap& rhs) const {
        const uip_t& _uip = rhs.get_uip();
        return ( uip.w64[1]!=_uip.w64[1] || uip.w64[0]!=_uip.w64[0] );
    }

    bool operator == (const ip_wrap& rhs) const {
        return !( operator != (rhs) );
    }

public: // family of methods to work with strings
    ip_wrap(const std::string& sip) {
        _str_to_ip(sip);
    }

    ip_wrap(const char* const sip) {
        _str_to_ip(sip);
    }

    ip_wrap& operator = (const std::string& sip) {
        _str_to_ip(sip);
        return *this;
    }

    const char* to_c_str(char* const buf6) const {
        memset(buf6, 0, INET6_ADDRSTRLEN);
        if ( IN6_IS_ADDR_V4MAPPED((const struct in6_addr *)&(uip.ip6))) {
            inet_ntop(AF_INET, &(uip.ip6.s6_addr32[3]), buf6, INET_ADDRSTRLEN);
        } else {
            inet_ntop(AF_INET6, &uip.ip6, buf6, INET6_ADDRSTRLEN);
        }
        return buf6;
    }

    operator std::string () const {
        //ATTENTION! it is for test only
        char buf6[INET6_ADDRSTRLEN];
        to_c_str(buf6);
        std::string sip(buf6);
        return sip;
    }


public: // family of methods to work with struct in_addr
    ip_wrap (const struct in_addr& ip) {
        invalidate();
        uip.w16[5] = 0xffff;
        uip.w32[3] = (uint32_t&)ip;
    }

    bool isIPv4() const {
        return IN6_IS_ADDR_V4MAPPED(&uip.ip6) ? true : false;
    }
    ip_wrap& operator = (const struct in_addr& ip) {
        invalidate();
        uip.w16[5] = 0xffff;
        uip.w32[3] = (uint32_t&)ip;
        return *this;
    }

    ip_wrap& operator = (const struct in_addr* const pip) {
        return operator = (*pip);
    }

public: // family of method to work with struct in6_addr
    ip_wrap (const struct in6_addr& ip) {
        memcpy(&uip, &ip, sizeof(in6_addr));
    }

    bool isIPv6() const {
        return !isIPv4();
    }

    ip_wrap& operator = (const struct in6_addr& ip) {
        memcpy(&uip, &ip, sizeof(in6_addr));
        return *this;
    }

    ip_wrap& operator = (const struct in6_addr* const pip) {
        return operator = (*pip);
    }

public: // family of methods to work with struct sockaddr
    ip_wrap( const struct sockaddr& rsa ) {
        if (rsa.sa_family != AF_INET) {
            if (rsa.sa_family != AF_INET6) {
                invalidate();
                LOG( LOG_ERROR, "zzz-elb %s neither the IPv4 nor the IPv6 wasn't detected", __func__);
            } else {
                memcpy(&uip, &((sockaddr_in6 *)&rsa)->sin6_addr, sizeof(in6_addr));
            }
        } else {
            invalidate();
            uip.w16[5] = 0xffff;
            //uip.w32[3] = (uint32_t&)((sockaddr_in *)&rsa)->sin_addr;
            memcpy(&(uip.w32[3]), &((sockaddr_in *)&rsa)->sin_addr, sizeof(in_addr));
        }
    }

    ip_wrap& operator = ( const struct sockaddr* const psa ) {
        if (psa->sa_family != AF_INET) {
            if (psa->sa_family != AF_INET6) {
                invalidate();
                LOG( LOG_ERROR, "zzz-elb %s neither the IPv4 nor the IPv6 wasn't detected", __func__);
            } else {
                return ( operator = (&(((struct sockaddr_in6*)psa)->sin6_addr)) );
            }
        }
        return ( operator = (&(((struct sockaddr_in*)psa)->sin_addr)) );
    }

    ip_wrap& operator = ( const struct sockaddr& rsa ) {
        return operator = ( (const struct sockaddr* const)&rsa);
    }

    bool update_in_sockaddr(struct sockaddr& rsa) const {
        if (isIPv4()) {
            if (rsa.sa_family == AF_INET6) {
                LOG( LOG_INFO, "zzz-elb IPv6 will be changed to IPv4");
            }
            rsa.sa_family = AF_INET;
            memcpy(&((sockaddr_in *)&rsa)->sin_addr, &(uip.w32[3]), sizeof(in_addr));
            return true;
        }

        if (rsa.sa_family == AF_INET) {
            LOG( LOG_INFO, "zzz-elb wIPv4 will be changed to IPv6");
        }
        rsa.sa_family = AF_INET6;
        memcpy(&(((struct sockaddr_in6*)&rsa)->sin6_addr), &uip, sizeof(in6_addr));
        return true;
    }

    //in case of FV sockaddr_in6 == sockaddr  but we must wrap next methods
    ip_wrap& operator = ( const struct sockaddr_in6& rsa ) {
        return operator = ( (const struct sockaddr*)&rsa);
    }

    ip_wrap& operator = ( const struct sockaddr_in6* const rsa ) {
        return operator = ( (const struct sockaddr* const)&rsa);
    }

    bool update_in_sockaddr(struct sockaddr_in6& rsa) const  {
        return  update_in_sockaddr ( (struct sockaddr&)rsa);
    }

public: // family of methods to work with struct sockaddr_storage
    ip_wrap( const struct sockaddr_storage& rsas ) {
        if (((struct sockaddr*)&rsas)->sa_family != AF_INET) {
            if (((struct sockaddr*)&rsas)->sa_family != AF_INET6) {
                invalidate();
                LOG( LOG_ERROR, "zzz-elb %s neither the IPv4 nor the IPv6 wasn't detected", __func__);
            } else {
                memcpy(&uip, &(((struct sockaddr_in6* const)&rsas)->sin6_addr), sizeof(in6_addr));
            }
        } else {
            invalidate();
            uip.w16[5] = 0xffff;
            memcpy(&(uip.w32[3]), &(((struct sockaddr_in* const)&rsas)->sin_addr), sizeof(in_addr));
        }
    }
    /*
        ip_wrap& operator = ( const struct sockaddr_storage* psas ) {
            if (((struct sockaddr*)psas)->sa_family != AF_INET){
                if (((struct sockaddr*)psas)->sa_family != AF_INET6) {
                _zeroed();
                LOG( LOG_ERROR, "zzz-elb %s neither the IPv4 nor the IPv6 wasn't detected", __func__);
                }
                return ( operator = (&(((struct sockaddr_in6*)psas)->sin6_addr)) );
            }
            return ( operator = (&(((struct sockaddr_in*)psas)->sin_addr)) );
        }

        ip_wrap& operator = ( const struct sockaddr_storage& rsas ) {
            return operator = ( (const struct sockaddr_storage* )&rsas);
        }

        bool update_in_sockaddr_storage(struct sockaddr_storage& rsas) const {

            if (rsas.ss_family != AF_INET6 && rsas.ss_family != AF_INET ) {
                // this method never return false for now
                // may be will be situation when need to check sockaddr_storage
                // this situation potential error
            }

            if (isIPv4()) {
                if (rsas.ss_family == AF_INET6) {
                    LOG( LOG_INFO, "zzz-elb IPv6 will be changed to IPv4");
                }
                rsas.ss_family = AF_INET;
                memcpy(&(((struct sockaddr_in*)&rsas)->sin_addr), &(uip.w32[3]), sizeof(in_addr));
                return true;
            }

            if (rsas.ss_family == AF_INET) {
                LOG( LOG_INFO, "zzz-elb wIPv4 will be changed to IPv6");
            }
            rsas.ss_family = AF_INET6;
            memcpy(&(((struct sockaddr_in6*)&rsas)->sin6_addr), &uip, sizeof(in6_addr));
            return true;
        }
    */
public: // family of methods to work with other data types
    //Mostly it is a hack, but it is need to simplify testing

    ip_wrap(unsigned int w32_3 = 0, unsigned int w32_2 = 0, unsigned int w32_1 = 0, unsigned int w32_0 = 0 ) {
        uip.w32[0] = htonl(w32_0);
        uip.w32[1] = htonl(w32_1);
        uip.w32[2] = htonl(w32_2);
        uip.w32[3] = htonl(w32_3);
    }

    void invalidate() {
        memset(&uip, 0, sizeof(uip));
    }

    const uip_t& get_uip() const {
        return uip;
    }

    bool is_valid(){
     return ( uip.w64[0] != 0 || uip.w64[1] != 0 ) ? true : false;
    }

    bool is_invalid(){
     return !is_valid();
    }

    /*TODO this methods may be useful in future
        ip_wrap& operator = (unsigned long x) {
            _zeroed();
            uip.w64[1] = htonl(x);
            return *this;
        }

        operator uint32_t() {
            return ntohl(uip.w32[3]);
        }

    */

private:

    int _str_to_ipv4(const std::string sip) {
        invalidate();
        uip.w16[5] = 0xffff;
        return inet_pton(AF_INET, sip.c_str(), &(uip.w32[3]));
    }

    int _str_to_ipv6(const std::string sip) {
        return inet_pton(AF_INET6, sip.c_str(), &uip.ip6);
    }

    int _str_to_ip(const std::string sip) {
        return (sip.find('.') != std::string::npos) ? _str_to_ipv4(sip)
               : _str_to_ipv6(sip);
    }

public:
    /*TODO
    ip_wrap(ip_wrap* prhs) {
        if (prhs == nullptr) {
            uip.w64[0] = uip.w64[0] = 0;
        } else {
            const uip_t _uip = prhs->get_uip();
            uip.w64[0] = _uip.w64[0];
            uip.w64[1] = _uip.w64[1];
        }
    }

        bool operator == (const ip_wrap* prhs) const {
            if (prhs == nullptr)
                return false;
            const uip_t _uip = prhs->get_uip();
            return ( uip.w64[0]==_uip.w64[0] && uip.w64[1]==_uip.w64[1] );
        }



    ip_wrap(uint64_t w0, uint64_t w1) {
        uip.w64[0] = w0;
        uip.w64[1] = w1;
    }


    BLA-BLA-BLA
    */
};


#endif
