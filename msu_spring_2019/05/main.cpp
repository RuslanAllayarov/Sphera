#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>

const std::uint64_t MAX = 1000000;
std::mutex mutex;
std::condition_variable cv;
std::uint64_t j = 0; // переменная итерации

void threadFunction_ping()
{
    std::unique_lock<std::mutex> locker(mutex);
    while(j < MAX)
    {   
        while (j % 2 == 0)
            cv.wait(locker);
        std::cout << "ping" << std::endl;
        j++;
        cv.notify_one();
    }
}
 

void threadFunction_pong()
{
    std::unique_lock<std::mutex> locker(mutex);
    while(j < MAX)
    {
        while (j % 2 == 1)
            cv.wait(locker);
        std::cout << "pong" << std::endl;
        j++;
        cv.notify_one();
    }
}
int main()
{
    std::thread thr_ping(threadFunction_ping);
    std::thread thr_pong(threadFunction_pong); 
    thr_ping.join();
    thr_pong.join();
    return 0;
}