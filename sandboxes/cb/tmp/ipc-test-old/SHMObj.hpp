#ifndef __SHMOBJ_HPP__
#define __SHMOBJ_HPP__

#include <unistd.h>
#include <sys/mman.h> //shm_open
#include <errno.h>

#define  BucketMalloc malloc
#define  BucketFree free

// **********************************************************************
// Shared Memory

#define SHM_NAME_LEN 256
struct SHMObj {
    int fd;
    void *mmapPtr;
    int mmapSize;
    char name[SHM_NAME_LEN];
};


int SHMExists( const char *name ) {
    char *shmName;
    int funcRet = 0;

    shmName = (char *)BucketMalloc( strlen( "/dev/shm/" ) + strlen( name ) + 1 );
    if ( !shmName ) {
        LOG( LOG_ERROR, "SHMExists: out of memory" );
        goto EXIT;
    }
    sprintf( shmName, "/dev/shm/%s", name );
    if ( access( shmName, F_OK ) != 0 ) {
        goto EXIT;
    }
    funcRet = 1;
EXIT:
    if ( shmName ) {
        BucketFree( shmName );
    }
    return funcRet;
}

// SHMOpenObj
// flags: passed to shm_open(). If O_CREAT and O_EXCL both exist and
// the shared memory file already exists, then the open it without
// initializing it. If only O_CREAT exists, then the memory location
// will be zero initialized.
int SHMOpenObj( const char *name,
                int flags,
                int dataSize,
                struct SHMObj *shmObj ) {
    char *data = NULL;
    int bytes;
    int funcRet = 0;
    int ret;
    int retried = false;

    while ( 1 ) {
        ret = shmObj->fd = shm_open( name, flags, 0660 );
        if ( ret == -1 ) {
            if ( !retried && errno == EEXIST ) {
                // if O_EXCL was specified then we will end up here which means
                // the file already exists, we don't need to initialize it.
                flags &= ~( O_CREAT | O_EXCL );
                retried = true;
                continue;
            } else {
                funcRet = -errno;
                if ( data ) {
                    BucketFree( data );
                }

                return funcRet;
            }
        }
        break;
    }
    if ( flags & O_CREAT ) {
        data = (char *)BucketMalloc( dataSize );
        memset( data, 0, dataSize );
        bytes = write( shmObj->fd, data, dataSize );
        if ( bytes != dataSize ) {
            if ( bytes == -1 ) {
                funcRet = -errno;
                LOG( LOG_ERROR, "Could not write to Shared Memory File: %s: %d: %s", name, errno, strerror(errno) );
            } else {
                LOG( LOG_ERROR, "Wrote only %d of %d expected bytes.", bytes, dataSize );
            }
            if ( data ) {
                BucketFree( data );
            }

            return funcRet;
        }
    }
    int mmap_flags = (flags & O_RDWR) ? PROT_READ | PROT_WRITE : PROT_READ ;
    shmObj->mmapPtr = mmap( NULL,
                            dataSize,
                            mmap_flags,
                            MAP_SHARED,
                            shmObj->fd,
                            0 );

    if ( shmObj->mmapPtr == MAP_FAILED ) {
        funcRet = -errno;
        LOG( LOG_ERROR, "Could not mmap Shared Memory File: %s: %d: %s", name, errno, strerror(errno));
        if ( data ) {
            BucketFree( data );
        }

        return funcRet;
    }

    shmObj->mmapSize = dataSize;
    memset( shmObj->name, 0, sizeof( shmObj->name ) );
    strncpy( shmObj->name, name, sizeof( shmObj->name ) -1 );

    funcRet = 1;

    if ( data ) {
        BucketFree( data );
    }

    return funcRet;
}

void SHMCloseFile( struct SHMObj *shmObj ) {
    if ( shmObj->fd != -1 ) {
        close( shmObj->fd );
        LOG( LOG_DEBUG, "Shared memory objects file successfully closed");
    }
    return;
}

void SHMCloseObj( struct SHMObj *shmObj ) {
    if ( shmObj->mmapPtr == NULL ) {
        LOG( LOG_ERROR, "Shared memory object is not initialized.");
        return;
    }
    if ( munmap( shmObj->mmapPtr, shmObj->mmapSize ) != 0 ) {
        LOG( LOG_ERROR, "Could not munmap() shared memory file: %s: %d: %s", shmObj->name, errno, strerror(errno));
        return;
    }

    SHMCloseFile(shmObj);
    LOG( LOG_DEBUG, "Shared memory object successfully uninitialized");

    return;
}
#endif
