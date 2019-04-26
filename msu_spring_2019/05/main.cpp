#include <thread>
#include <iostream>
#include <atomic>

static const size_t MAX = 1000000;
std::mutex mutex;
static std::atomic_int64_t j{0};


void threadFunction(size_t ind)
{
    while (true)
    {
        if (j >= MAX)
            break;
        if ((ind == 0) && (j % 2 == 0))
        {
            std::cout << "ping" << std::endl;
            j++;
        }
        if ((ind == 1) && (j % 2 == 1))
        {
            std::cout << "pong" << std::endl;
            j++;
        }
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
