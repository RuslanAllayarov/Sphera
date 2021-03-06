#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>
#include <condition_variable>

std::condition_variable cv;
std::mutex mutex;
static const size_t MAX = 1000000;
static std::atomic_int64_t j{0};


void threadFunction(bool ind)
{
    const char* str = ind ? "ping" : "pong";
    while (j < MAX - 1)
    {
        std::unique_lock<std::mutex> locker(mutex);
        cv.wait(locker, [ind]{
            return ((!ind) && (j % 2)) || (ind && !(j % 2));
            });
        std::cout << str << std::endl;
        j++;
        cv.notify_one();
    }
} 

int main()
{
    std::thread thr_ping(threadFunction, 0);
    std::thread thr_pong(threadFunction, 1); 
    thr_ping.join();
    thr_pong.join();
    return 0;
}
