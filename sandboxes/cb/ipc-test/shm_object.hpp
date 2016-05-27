#ifndef shm_obj_HPP_INCLUDED
#define shm_obj_HPP_INCLUDED

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> //for O_RDONLY
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#ifndef nullptr
#define nullptr NULL
#endif

class shm_file {

private:
    const std::string full_name;
    int flags;
    int fd;

public:
    shm_file(const char *_name,int _flags):
        full_name(std::string("/dev/shm/")+_name), flags(_flags), fd(-1) {
    }

    virtual ~shm_file() {
        if (fd != -1) {
            ::close(fd);
            shm_unlink(full_name.c_str());
            fd = -1;
        }
    }

    int get_fd() {
        if ( fd <= 0 ) {
            fd = shm_file::open(full_name, flags);
        }
        return fd;
    };
//static part
public:
    static off_t length(const std::string& nm) {
        struct stat sstat;
        if (stat(nm.c_str(),&sstat) == -1) {
            shm_file::prn_errno(errno, nm.c_str());
        }
        fprintf(stdout, "length of %s sstat:\n  st_mode=%x\n  st_size=%d[%x]\n",
                nm.c_str(), sstat.st_mode, sstat.st_size, sstat.st_size);
        return sstat.st_size;
    }

    static off_t length(int fd) {
        assert( fd <= 0 );
        struct stat sstat;
        if (fstat(fd,&sstat) == -1) {
            shm_file::prn_errno(errno);
        }
        fprintf(stdout, "length fd=%d sstat:\n  st_mode=%x\n  st_size=%d[%x]\n",
                fd, sstat.st_mode, sstat.st_size, sstat.st_size);
        return sstat.st_size;
    }

    static size_t truncate(const std::string& nm, off_t sz) {
        if (sz != shm_file::length(nm))
            if (truncate(nm, sz) == -1) {
                shm_file::prn_errno(errno, nm.c_str());
            }
        return shm_file::length(nm);
    }

    static size_t truncate(int fd, off_t sz) {
        assert( fd <= 0 );
        if (sz != shm_file::length(fd))
            if (ftruncate(fd, sz) == -1) {
                shm_file::prn_errno(errno);
            }
        return shm_file::length(fd);
    }

    static int open(const std::string& nm, int _flags) {
        struct stat sstat;
        if (stat(nm.c_str(), &sstat) == -1)
            if (errno == ENOENT && (_flags & (O_RDWR | O_CREAT))) {
                _flags |= O_CREAT;
            } else {
                shm_file::prn_errno(errno, nm.c_str());
                return -1;
            }
        int fd = shm_open(nm.c_str(), _flags, S_IRWXU|S_IRWXG); //0660=//S_IRWXU|S_IRWXG//|S_IRWXO
        if (fd == -1 ) {
            shm_file::prn_errno(errno, nm.c_str());
        }
        return fd;
    }

    static void prn_errno(int _errno, const char* nm = nullptr ) {
        if ( _errno == EINVAL )
            fprintf(stdout,"%d",_errno);
        //TODO print something with errno
        /*
        EACCES            Permission to shm_unlink() the shared memory object was denied.
        EACCES            Permission was denied to shm_open() name in the specified mode, or O_TRUNC was specified and the caller does not have write permission on the object.
        EEXIST            Both O_CREAT and O_EXCL were specified to shm_open() and the shared memory object specified by name already exists.
        EINVAL            The name argument to shm_open() was invalid.
        EMFILE            The process already has the maximum number of files open.
        ENAMETOOLONG      The length of name exceeds PATH_MAX.
        ENFILE            The limit on the total number of files open on the system has been reached.
        ENOENT            An attempt was made to shm_open() a name that did not exist, and O_CREAT was not specified.
        ENOENT            An attempt was to made to shm_unlink() a name that does not exist.
        */
    }

};

template<typename TTable>
class shm_obj {
    TTable* pshmt;
    shm_file file;

public:

    shm_obj(const char*_name): pshmt(nullptr), file(_name)  {
    };

    TTable* get_pshmt() {
        if (pshmt != nullptr)
            return pshmt;

        int fd = file.get_fd( (mmap_flags() & PROT_WRITE) ? O_RDWR : O_RDONLY );

        shm_file::truncate( fd, sizeof(TTable) );

        void* p = mmap(nullptr, sizeof(TTable), mmap_flags(), MAP_SHARED, fd, 0);
        if (p != MAP_FAILED) {
            pshmt = (TTable*)p ;//new(p) TTable;
            //TODO zzz file.close_fd()
            return pshmt; // success exit;
        }
        //TODO zzz print error
        return nullptr;
    }

    virtual ~shm_obj() {
        unmap(); //TODO zzz check sizeof
    }

private:
    virtual int mmap_flags() = 0;

    virtual void unmap() {
        if ( pshmt != nullptr ) {
            munmap(pshmt, sizeof(TTable));
            pshmt = nullptr;
        }
    }

    /*bool is_file_exists(const char* path) {
        return std::ifstream(&path) != nullptr;
    }*/
};

template <typename TTable>
class shm_obj_reader : public shm_obj<TTable> {
public:
    shm_obj_reader(const char*_name):shm_obj<TTable>(_name) {}
private:
    virtual int mmap_flags () {
        return PROT_READ;
    }
};

template <typename TTable>
class shm_obj_writer : public shm_obj<TTable> {

public:
    shm_obj_writer(const char*_name):shm_obj<TTable>(_name) {}
private:
    virtual int mmap_flags () {
        return PROT_READ|PROT_WRITE;
    }
};

#endif
