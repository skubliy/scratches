#include <thread>
#include <atomic>
#include <stdio.h>
#include "RandomDelay.h"

using namespace std;

atomic<int> flag;
uint64_t sharedValue = 0;
uint8_t iflag;

bool use_atomic = false;

RandomDelay randomDelay1(1, 60101);
RandomDelay randomDelay2(2, 65535);

void IncrementSharedValue10000000Times(RandomDelay& randomDelay) {
    int count = 0;
    while (count < 10000000) {
        //randomDelay.doBusyWork();
        if (use_atomic) {
            int expected = 0;
            if (flag.compare_exchange_strong(expected, 1, memory_order_relaxed)) {
                //flag.store(1, memory_order_relaxed);
                // Lock was successful
                sharedValue++;
                flag.store(0, memory_order_relaxed);
                count++;
            }
        } else {
            if ( iflag == 0 ) {
                iflag = 1;
                sharedValue++;
                iflag = 0;
                count++;
            }
        }
    }
}

void Thread2Func() {
    IncrementSharedValue10000000Times(randomDelay2);
}

int main(int argc, char* argv[]) {

    use_atomic = (argc > 1) ? true : false ;

    printf("is_lock_free: %s. is using atomic? %s \n",
        flag.is_lock_free() ? "true" : "false",
        use_atomic ? "true" : "false"
        );

    for (;;) {
        sharedValue = 0;
        thread thread2(Thread2Func);
        IncrementSharedValue10000000Times(randomDelay1);
        thread2.join();
        printf("sharedValue=%lu\n", sharedValue);
    }

    return 0;
}

