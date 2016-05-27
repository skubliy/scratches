#ifndef SHM_OBJ_HPP_INCLUDED
#define SHM_OBJ_HPP_INCLUDED

#include <stdio.h> // sprintf

#include <sys/mman.h>   // shm_open
#include <sys/stat.h>   // fstat, stat ...
#include <unistd.h>     // ftruncate
#include <fcntl.h>      // definition of O_XX constants: O_RDONLY, O_RDWR, O_CREATE ...
#include <string.h>     // strlen
#include <errno.h>      // errno

template <typename T>
struct shm_obj {
    typedef shm_obj<T>  _slf_t;
    typedef T*          _ptr_t;
    typedef T&          _ref_t;

    T *pmmap;
    int fd;
    int fmode;

    enum { MAX_NAME_LENGTH = 256 };
    char fname[MAX_NAME_LENGTH];

public:
    // shm_obj
    // flags: passed to shm_open(). If O_CREAT and O_EXCL both exist and
    // the shared memory file already exists, then the open it without
    // initializing it. If only O_CREAT exists, then the memory location
    // will be zero initialized.
    shm_obj( const char* const mid, const char* const suffix, int _flags=O_RDONLY) :
        pmmap(NULL), fd(-1), fmode(_flags) {

        // crating shmem file name
        static const char* const prefix =  "_shmo-";

        if ( (strlen( prefix ) + strlen( mid ) + strlen( suffix ) + 1 ) > MAX_NAME_LENGTH ) {
            LOG( LOG_ERROR, "Too long name" );
            return;
        }
        memset(fname, 0, MAX_NAME_LENGTH);
        sprintf( fname, "%s%s%s", prefix, mid, suffix);

        bool retried = false;
        while ( 1 ) {
            fd = shm_open( fname, fmode, 0660 ); //0660==S_IRWXU|S_IRWXG//|S_IRWXO
            if ( fd == -1 ) {
                if ( !retried && errno == EEXIST ) {
                    // if O_EXCL was specified then we will end up here which means the file
                    // already exists, and we not "owner/creator" don't need to initialize it.
                    // If we are "owner/creator" need to unlink, reopen and reinitialize it
                    // but it is wrong situation, need at list print in log something
                    if ( (fmode | O_CREAT) && (fmode | O_RDWR) ) {
                        LOG( LOG_ERROR, "Could not fstat file %s fd(%d) errno[%d] %s",
                             fname, fd, errno, strerror(errno));
                        unlink_file();
                    } else // O_RDONLY reader not owner and can't reopen and reinitialize shm_obj
                        //FIXME really need to initialized memory every time when writer restart
                        fmode &= ~( O_CREAT | O_EXCL );

                    retried = true;
                    continue;
                } else {
                    LOG( LOG_ERROR, "Could not shm_open file %s errno[%d] %s",
                         fname, errno, strerror(errno));
                    return;
                }
            }
            break;
        }

        if ( fmode & O_CREAT ) {
            struct stat sstat;
            if ( fstat(fd, &sstat) == -1 ) {
                LOG( LOG_ERROR, "Could not fstat file %s fd(%d) errno[%d] %s",
                     fname, fd, errno, strerror(errno));
                return;
            }
            if (sstat.st_size != sizeof(T))
                if (ftruncate(fd, sizeof(T)) == -1) {
                    LOG( LOG_ERROR, "Could not ftruncate file %s fd(%d) to size %lu errno[%d] %s",
                         &fname[0], fd, sizeof(T), errno, strerror(errno));
                    return ;
                }
        }

        _flags = ( _flags & O_RDWR ) ? PROT_READ | PROT_WRITE : PROT_READ ;
        void* p = mmap( NULL, sizeof(T), _flags, MAP_SHARED, fd, 0 );
        if ( p == MAP_FAILED ) {
            LOG( LOG_ERROR, "Could not mmap file %s fd(%d) errno[%d] %s",
                 fname, fd, errno, strerror(errno));
        }

        //FIXME really need to initialized memory every time when writer restart
        // but we must do initialization
        if ( fmode & O_CREAT ) {
            // Initialization
            memset( p, 0, sizeof( T ) );
        }

        pmmap = reinterpret_cast<T*>(p);

        if ( fd > 0 ) { //We can close file now
            ::close( fd );
            fd = 0;
            LOG( LOG_DEBUG, "Shared memory objects file successfully closed");
        }
    }

    virtual ~shm_obj() {
        if ( pmmap == NULL ) {
            LOG( LOG_ERROR, "Shared memory object is not initialized.");
            return;
        }
        if ( munmap( (void*)pmmap, sizeof(T)) != 0 ) {
            LOG( LOG_ERROR, "Could not munmap() shared memory file: %s: %d: %s",
                 fname, errno, strerror(errno));
            return;
        }

        if ( fd > 0 ) {
            ::close( fd );
            fd = 0;
            LOG( LOG_INFO, "zzz some thing strange here Shared memory objects file successfully closed");
        }

        if ( fmode & O_CREAT )
            unlink_file();

        LOG( LOG_INFO, "Shared memory object successfully uninitialized");

        return;
    }


    //TODO it is public for now
    operator _ref_t() const {
        return reinterpret_cast<T&>(*pmmap);
    }

    operator _ptr_t() {
        return (pmmap);
    }

private:

    void unlink_file() {
        char buf[256];
        memset(buf,0,sizeof(buf));
        sprintf(buf,"/dev/shm/%s", fname);
        unlink(&buf[0]);
    }
};

#endif




