#include <thread>
#include <atomic>
#include <stdio.h>

using namespace std;

atomic<int> flag;
uint64_t sharedValue = 0;
uint8_t iflag;

bool use_atomic = false;

void IncrementSharedValue10000000Times(void) {
    int count = 0;
    while (count < 10000000) {
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
    IncrementSharedValue10000000Times();
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
        IncrementSharedValue10000000Times();
        thread2.join();
        printf("sharedValue=%lu\n", sharedValue);
    }

    return 0;
}

