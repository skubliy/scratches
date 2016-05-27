#include "dn_cache.h"

char* dn_record::get_domain_name(){
        return &domain_name_record[0];
        }

void dn_record::set_domain_name(const char* _name){
        strcpy(domain_name_record, _name);
        }

    void dn_record::set_domain_name(const char* _name, uint len){
        memcpy(domain_name_record, _name, len);
        domain_name_record[len] = 0;
        }

    TSAR& dn_record::operator[](uint _num) {
        if (_num < MAX_IPADDRS_PER_DOMAIN_NAME)
            return socket_addr_records[_num];
        return NULL;
        }


dn_cache::dn_cache()
{
    //ctor
}

dn_cache::~dn_cache()
{
    //dtor
}

dn_cache::dn_cache(const dn_cache& other)
{
    //copy ctor
}

dn_cache& dn_cache::operator=(const dn_cache& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
