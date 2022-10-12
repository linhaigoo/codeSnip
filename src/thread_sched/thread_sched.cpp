// https://stackoverflow.com/questions/63674522/linux-sched-fifo-not-respecting-thread-priorities

#include <chrono>
#include <cstring>
#include <future>
#include <iostream>
#include <math.h>
#include <mutex>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <thread>
#include <unistd.h>

// IO Access mutex
std::mutex g_mutex_io;

// Computation function (busy work)
static bool isPrime(unsigned int value)
{
    unsigned int i, root;
    if (value == 1)
        return false;
    if (value == 2)
        return true;
    if ((value % 2) == 0)
        return false;
    root = (int)(1.0 + sqrt(value));
    for (i = 3; (i < root) && (value % i != 0); i += 2)
        ;
    return (i < root ? false : true);
}

// Thread function
void foo(unsigned int id, unsigned int count)
{
    sched_param sch;
    int policy, sum = 0;

    // // Get information about thread
    // pthread_getschedparam(pthread_self(), &policy, &sch);

    // Compute primes
    for (unsigned int i = 1; i < 1000; ++i)
    {
        sum += (isPrime(i) ? 1 : 0);
    }
    setpriority(PRIO_PROCESS, syscall(SYS_gettid), 10);
    for (unsigned int i = 1; i < count; ++i)
    {
        sum += (isPrime(i) ? 1 : 0);
    }

    // Print
    {
        // Get information about thread
        pthread_getschedparam(pthread_self(), &policy, &sch);
        std::lock_guard<std::mutex> lock(g_mutex_io);
        std::cout << "Thread " << id << " summed " << sum << " primes"
                  << " at priority level: " << sch.sched_priority << std::endl;
        printf("getpid: %d getpthread_self: %lu tid:%lu\n", getpid(), pthread_self(), syscall(SYS_gettid));
    }
}

int main()
{
    printf("getpid: %d getpthread_self: %lu tid:%lu\n", getpid(), pthread_self(), syscall(SYS_gettid));
    sched_param sch;
    int policy;

    // Declare and init task objects
    std::packaged_task<void(unsigned int, unsigned int)> task_1(foo);
    std::packaged_task<void(unsigned int, unsigned int)> task_2(foo);

    // Get the futures
    auto task_fut_1 = task_1.get_future();
    auto task_fut_2 = task_2.get_future();

    // Declare and init thread objects
    std::thread thread_1(std::move(task_1), 1, 500000000);
    std::thread thread_2(std::move(task_2), 2, 500000000);

    // Set first thread policy
    pthread_getschedparam(thread_1.native_handle(), &policy, &sch);
    sch.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (pthread_setschedparam(thread_1.native_handle(), SCHED_FIFO, &sch))
    {
        std::cerr << "pthread_setschedparam: " << std::strerror(errno) << std::endl;
        return -1;
    }

    // Set second thread policy
    pthread_getschedparam(thread_2.native_handle(), &policy, &sch);
    sch.sched_priority = 20;
    if (pthread_setschedparam(thread_2.native_handle(), SCHED_FIFO, &sch))
    {
        std::cerr << "pthread_setschedparam: " << std::strerror(errno) << std::endl;
        return -1;
    }

    // Set main process thread priority
    pthread_getschedparam(pthread_self(), &policy, &sch);
    sch.sched_priority = 0;
    if (pthread_setschedparam(pthread_self(), SCHED_OTHER, &sch))
    {
        std::cerr << "pthread_setschedparam: " << std::strerror(errno) << std::endl;
        return -1;
    }

    // Detach these threads
    thread_1.detach();
    thread_2.detach();

    // Check their status with a timeout
    for (int finished1 = 0, finished2 = 0; !(finished1 && finished2);)
    {
        if (task_fut_1.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
            {
                std::lock_guard<std::mutex> lock(g_mutex_io);
                std::cout << "Main: Task 1 has finished!" << std::endl;
            }
            finished1++;
        }
        if (task_fut_2.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
            {
                std::lock_guard<std::mutex> lock(g_mutex_io);
                std::cout << "Main: Task 2 has finished!" << std::endl;
            }
            finished2++;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            std::lock_guard<std::mutex> lock(g_mutex_io);
            std::cout << "Main: Checking ..." << finished1 << "  " << finished2 << std::endl;
        }
    }
    pthread_getschedparam(pthread_self(), &policy, &sch);
    std::cout << "Main: Exiting at priority level: " << sch.sched_priority << std::endl;
    return 0;
}
