#include <iostream>
#include <thread>

class my_thread : private std::thread
{
  private:
    const bool _is_join;

  public:
    using std::thread::join;
    using std::thread::joinable;
    template <typename... T> my_thread(bool is_join, T &&...para) : thread(std::forward<T>(para)...), _is_join(is_join)
    {
        std::cout << "is_join: " << is_join << std::endl;
    }

    ~my_thread()
    {
        if (this->joinable())
        {
            if (_is_join)
            {
                this->join();
            }
            else
            {
                this->detach();
            }
        }
    }
};

class lambda_convert_test
{
  private:
    bool test;

  public:
    explicit lambda_convert_test(bool test)
    {
    }

    explicit lambda_convert_test(float test)
    {
    }
    ~lambda_convert_test() = default;
};

int main()
{
    auto ret = []() { std::cout << "1234" << std::endl; };

    // my_thread thread1(ret); // compiled,but core dumped
    my_thread thread2(false);
    // my_thread thread3([]() { std::cout << "12345" << std::endl; }); // compiled,but core dumped
    my_thread thread4(false, ret);
    my_thread thread5(false, [&ret]() { std::cout << "1234" << std::endl; });
    // my_thread thread6([&ret]() { std::cout << "1234" << std::endl; });//cannot comiled

    lambda_convert_test lambda_test1(10.0f);
    lambda_convert_test lambda_test2(ret);

    return 0;
}
