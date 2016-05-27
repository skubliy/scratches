#ifndef DN_CACHE_H
#define DN_CACHE_H

template <typedef TSAR>
class dn_record{

    char domain_name[256];

    enum { MAX_IPADDRS_PER_DOMAIN_NAME=(8)};
    TSAR socket_addr_records[MAX_IPADDRS_PER_DOMAIN_NAME];

public:
    char* get_domain_name();
    void set_domain_name(const char* _name);
    void set_domain_name(const char* _name, uint len);
    TSAR& operator[](uint _num);

};

class dn_cache
{
    private:
        dn_cache();
        virtual ~dn_cache();
        dn_cache(const dn_cache& other){};
        dn_cache& operator=(const dn_cache& other){};
    protected:
    private:
};

#endif // DN_CACHE_H
