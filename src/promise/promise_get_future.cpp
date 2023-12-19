#include <chrono>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

void accumulate(std::vector<int>::iterator first, std::vector<int>::iterator last, std::promise<int> accumulate_promise)
{
    auto iter = first;
    // for (; iter != last; iter++)
    {
        accumulate_promise.set_value(*iter); // Notify future
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void do_work(std::promise<void> barrier)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    barrier.set_value();
}

int main()
{
    // Demonstrate using promise<int> to transmit a result between threads.
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
    std::promise<int> accumulate_promise;
    std::future<int> accumulate_future = accumulate_promise.get_future();
    std::thread work_thread(accumulate, numbers.begin(), numbers.end(), std::move(accumulate_promise));

    // future::get() will wait until the future has a valid result and retrieves it.
    // Calling wait() before get() is not needed
    // accumulate_future.wait();  // wait for result
    std::cout << "result=" << accumulate_future.get() << '\n';
    // accumulate_future.wait();
    // std::cout << "result=" << accumulate_future.get() << '\n';
    // accumulate_future.wait();
    // std::cout << "result=" << accumulate_future.get() << '\n';
    work_thread.join(); // wait for thread completion

    // Demonstrate using promise<void> to signal state between threads.
    std::promise<void> barrier;
    std::future<void> barrier_future = barrier.get_future();
    std::thread new_work_thread(do_work, std::move(barrier));
    barrier_future.wait();
    new_work_thread.join();
    {
        std::thread test([]() {
            std::this_thread::sleep_for(std::chrono::seconds(30));
            std::cout << "test thread will terminal \n";
        });
        test.detach();
    }
    uint64_t sum = 0;
    for (uint64_t i = 0; i < 0xfffffffff; i++)
    {
        sum += i;
    }
    // std::this_thread::sleep_for(std::chrono::seconds(30));
    std::cout << "main thread will terminal sum " << sum << "\n";
}