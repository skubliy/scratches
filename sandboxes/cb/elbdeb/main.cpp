#include <iostream>
#include <thread>
#include <atomic>
#include <stdio.h>
/*
template <typename T>
class invalid_flags{
    T* pf;
public:

    void set(int n){ }
    void reset(int n){ }
    bool is_set(int n){ return (pf[n/sizeof(T)]&(n%sizeof(T)))>0?true:false;}

private:
    T _mask;
}
*/

#define MAX1 (64)
#define MAX2 (16)

template <typename TFlag>
class invalid_flags {

    TFlag array_iflags[MAX1*MAX2];
    std::atomic<T> version;
public:
    invalid_flags() {}

    void set(int sid, int n) {
        array_iflags[n] | (1<<sid)
    }
    void reset(int sid, int n) {
        array_iflags[n] & ~(1 << sid)
    }
    bool is_set(int sid, int n) {
        return ( array_iflags[n] & (1 << sid) ) > 0 ? true : false;
    }
private:
    uint64_t _mask(int sid) {
        return 1 << sid;
    }

}


template <typename T>
class version_record : public std:atomic<T> {}

typedef version_record<uint32_t> ui32t_version;


/*
    std::atomic<T> version;
public:
    const data_version<T>& operator++();  // prefix

private:
    const data_version<T> operator++ (T){}; // postfix

}
*/



template <typedef T>
class data_value {
    T value;
public:

}

template<typename TRecord, TNum>
class shmem {

protected:
    int fd;
    TRecord* records;
    const char* name;

private:
    shmem(const shmem&);
    shmem &operator=(const shmem&);

public:
    virtual ~shmem() {
        //if (name)shm_unlink(name);
    }

    shmem():
        fd(0),
        records(NULL),
        name(NULL) {
    }

    TRecord&  operator[](TNum num) {
        //TODO check the range
        return records[num];
    }

    const TRecord&  operator[](TNum num) const {
        //TODO check the range
        return records[num];
    }

    TRecord* shmem_open(const char* _name, TNum _num_records)=0;
}


template<typename TTable, TNum>
class shparams {

protected:
    TTable& table;

private:
    shparams(const shparams&);
    shparams &operator=(const shparams&);

public:
    virtual ~shmem() {
        //if (name)shm_unlink(name);
    }

    shparams(TTable& _table):
        table(_table) {
    }



};



/*

    TRecord&  operator[](TNum num) = 0;

    const TRecord&  operator[](TNum num) const {
        //TODO check the range
        return records[num];
    }

    TRecord* shmem_open(const char* _name,TNum _num_records) {
        assert(_name);
        name=_name;
        //MDTRACE(_name,O_RDWR);
        // opening shared memory
        fd = shm_open(_name, O_RDWR, S_IRWXU|S_IRWXG|S_IRWXO);
        if (fd==-1) {
            if (errno==ENOENT) {
                MDTRACE("shmem O_CREAT ",O_RDWR|O_CREAT);
                fd = shm_open(_name, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
                if (fd==-1) {
                    serror("shm_open error");
                    //TODO need to fix
                }
                do_set_size(fd);
                do_mmap();
                do_init();
            }
            do_check(fd);
        } else {
            do_set_size(fd);
            do_mmap();
        }
        // Closing shared memory object
        close(fd);
        fd=0;
        return records;
    }

private:
    void do_mmap() {
        assert(fd);
        fchmod(fd, (S_IRWXU|S_IRWXG|S_IRWXO)); // g/u
        // Mmaping memory
        struct stat sstat;
        fstat(fd, &sstat);
        records = static_cast<TRecord*>(mmap(0,sstat.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0));
        if (records == reinterpret_cast<TRecord*>(-1))
            serror(" mmap failed ");
    }

    void do_set_size(int fd) {
        // Seting memory size
        if (ftruncate(fd,sizeof(TRecord)*SHMEM_TABLE_LEN)==-1)
            serror("set shm size error");
    }

//const char* get_default_name(){return SHMname;}
    void do_check(int fd) {
        if (fd<=0) {
            close(fd);
            serror("can't open shared memory");
        }
    }
};
*/



using namespace std;

int main() {
    cout << "Hello world!" << endl;
    return 0;
}
